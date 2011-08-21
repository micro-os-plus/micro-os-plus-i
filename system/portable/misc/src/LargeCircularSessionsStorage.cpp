/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_LARGECIRCULARSESSIONSSTORAGE)

#include "portable/misc/include/LargeCircularSessionsStorage.h"

// ============================================================================

// ----- Constructors & destructors -------------------------------------------

LargeCircularSessionsStorage::LargeCircularSessionsStorage(
    OSDeviceBlock& device) :
  m_device(device)
{
  OSDeviceDebug::putConstructor_P(PSTR("LargeCircularSessionsStorage"), this);

  m_blockSizeBlocks = 1;
}

LargeCircularSessionsStorage::~LargeCircularSessionsStorage()
{
  OSDeviceDebug::putDestructor_P(PSTR("LargeCircularSessionsStorage"), this);
}

// ----- Public methods -------------------------------------------------------

LargeCircularSessionsStorage::SessionBlockNumber_t
LargeCircularSessionsStorage::getStorageSizeSessionBlocks(void)
{
  // Compute the number of session block
  return getDevice().getDeviceSizeBlocks() / getSessionBlockSizeBlocks();
}

LargeCircularSessionsStorage::SessionBlockNumber_t
LargeCircularSessionsStorage::computeCircularSessionBlockNumber(
    SessionBlockNumber_t blockNumber, int adjustment)
{
  SessionBlockNumber_t size;
  size = getStorageSizeSessionBlocks();

#if false
  OSDeviceDebug::putString("size=");
  OSDeviceDebug::putDec(size);
  OSDeviceDebug::putNewLine();
#endif

  if (adjustment > 0)
    {
      return (blockNumber + adjustment) % size;
    }
  else
    {
      if (blockNumber >= (SessionBlockNumber_t) (-adjustment))
        {
          return blockNumber + adjustment;
        }
      else
        {
          return blockNumber + size + adjustment;
        }
    }
}

OSReturn_t
LargeCircularSessionsStorage::readStorageBlock(
    SessionBlockNumber_t blockNumber, uint8_t* pSessionBlock,
    OSDeviceBlock::BlockCount_t deviceBlocksCount)
{
  // Validate block count
  if (deviceBlocksCount > getSessionBlockSizeBlocks())
    return OSReturn::OS_OUT_OF_RANGE;

#if defined(OS_DEBUG_LARGECIRCULARSESSIONSTORAGE_READSTORAGEBLOCK)
  OSDeviceDebug::putString("readStorageBlock() blk=");
  OSDeviceDebug::putDec(blockNumber);
  OSDeviceDebug::putNewLine();
#endif /* OS_DEBUG_LARGECIRCULARSESSIONSTORAGE_READSTORAGEBLOCK */

  return getDevice().readBlocks(blockNumber * getSessionBlockSizeBlocks(),
      pSessionBlock, deviceBlocksCount);
}

OSReturn_t
LargeCircularSessionsStorage::writeStorageBlock(
    SessionBlockNumber_t blockNumber, uint8_t* pSessionBlock,
    OSDeviceBlock::BlockCount_t deviceBlocksCount)
{
  // Validate block count
  if (deviceBlocksCount > getSessionBlockSizeBlocks())
    return OSReturn::OS_OUT_OF_RANGE;

  return getDevice().writeBlocks(blockNumber * getSessionBlockSizeBlocks(),
      pSessionBlock, deviceBlocksCount);
}

OSReturn_t
LargeCircularSessionsStorage::searchMostRecentlyWrittenBlock(uint8_t* pBlock,
    SessionBlockNumber_t* pMostRecentBlock,
    LargeCircularSessionsStorage::Header& header)
{
#if defined(OS_INCLUDE_LARGECIRCULARSESSIONSSTORAGE_SINGLE)

  // Force the caller to start from the beginning of the storage
  return OSReturn::OS_NOT_FOUND;

#elif defined(OS_INCLUDE_LARGECIRCULARSESSIONSSTORAGE_LINIAR)

  Header left;
  Header right;
  Header middle;

  SessionBlockNumber_t leftBlockNumber;
  SessionBlockNumber_t rightBlockNumber;
  SessionBlockNumber_t middleBlockNumber;

  leftBlockNumber = 0;
  rightBlockNumber = getStorageSizeSessionBlocks() - 1;

  OSReturn_t ret;

  ret = readStorageBlock(leftBlockNumber, pBlock, 1);
  if (ret != OSReturn::OS_OK)
    {
      OSDeviceDebug::putChar('V');
      OSDeviceDebug::putHex((unsigned short) ret);
      return ret;
    }
  left.readFromBlock(pBlock);

  if (left.getMagic() != Header::MAGIC)
    {
      // If the first block does not have a magic, then we consider
      // the entire partition to be empty.
      return OSReturn::OS_NOT_FOUND;
    }

  ret = readStorageBlock(rightBlockNumber, pBlock, 1);
  if (ret != OSReturn::OS_OK)
    {
      OSDeviceDebug::putChar('W');
      OSDeviceDebug::putHex((unsigned short) ret);
      return ret;
    }
  right.readFromBlock(pBlock);

  if (right.getMagic() != Header::MAGIC)
    {
      // If the last block does not have a magic, then we consider
      // the entire partition to be linear, without roll, so we just
      // do a binary search of the last block with valid magic

      for (;;)
        {
          OSDeviceDebug::putString(" search ");
          OSDeviceDebug::putDec(leftBlockNumber);
          OSDeviceDebug::putChar('-');
          OSDeviceDebug::putDec(rightBlockNumber);
          OSDeviceDebug::putNewLine();

          if (leftBlockNumber + 1 == rightBlockNumber)
            {
              // Found, the info in the 'first' header is the one
              break;
            }

          middleBlockNumber = (leftBlockNumber + rightBlockNumber) / 2;
          ret = readStorageBlock(middleBlockNumber, pBlock, 1);
          if (ret != OSReturn::OS_OK)
            {
              OSDeviceDebug::putChar('X');
              OSDeviceDebug::putHex((unsigned short) ret);
              return ret;
            }
          middle.readFromBlock(pBlock);

          if (middle.getMagic() != Header::MAGIC)
            {
              // Middle header invalid, consider it as last
              rightBlockNumber = middleBlockNumber;
              right = middle;
            }
          else
            {
              // Midlle in use, adjust first
              leftBlockNumber = middleBlockNumber;
              left = middle;
            }
        }

      // Found, the first block is the most recently used
      // The last is the first empty block, with invalid magic

      *pMostRecentBlock = leftBlockNumber;
      header = left;
      return OSReturn::OS_OK;
    }

  OSDeviceDebug::putString("Storage is full, to be implemented shortly");
  OSDeviceDebug::putNewLine();

  ret = OSReturn::OS_NOT_FOUND;
  return ret;

#else

  // max block number, i.e. size -1
  unsigned long max;
  max = getStorageSizeSessionBlocks() - 1;

  uint8_t* pBlock;
  pBlock = m_block;

  BlockUniqueId_t id_beg; // First block ID
  id_beg = 0;

  BlockUniqueId_t id_last; // Last used block ID
  SessionBlockNumber_t blk_crt; // Current Block number

  SessionBlockNumber_t blk_beg; // Begin address (block)
  blk_beg = 0;

  SessionBlockNumber_t blk_max; // Max Block number
  blk_max = max;

  Header left;
  Header right;
  Header middle;

  SessionBlockNumber_t leftBlockNumber, rightBlockNumber, middleBlockNumber;

  bool ready; // true after mmc_init()
  ready = false;

  // search for last written block
  bool rollover;
  rollover = false;

  OSReturn_t r;

  unsigned char i;
  for (i = 0; i < 2; ++i)
    {
      leftBlockNumber = 0;
      rightBlockNumber = blk_max;
      //r = readBlkHdr(pBuf, blk_i, &id_i, &begblk_i);
      r = readStorageBlock(leftBlockNumber, pBlock, 1);
      if (r != OSReturn::OS_OK)
        {
          if (os.isDebug())
            {
              OSDeviceDebug::putChar('V');
              OSDeviceDebug::putHex((unsigned short) r);
            }
          return r;
        }
      //id_i = Header::readBlockUniqueId(pBuf);
      //begblk_i = Header::readSessionFirstBlockNumber(pBuf);
      left.readFromBlock(pBlock);

      //r = readBlkHdr(pBuf, blk_j, &id_j, &begblk_j);
      r = readStorageBlock(rightBlockNumber, pBlock, 1);
      if (r != OSReturn::OS_OK)
        {
          if (os.isDebug())
            {
              OSDeviceDebug::putChar('W');
              OSDeviceDebug::putHex((unsigned short) r);
            }

          return r;
        }
      //id_j = Header::readBlockUniqueId(pBuf);
      // begblk_j = Header::readSessionFirstBlockNumber(pBuf);
      right.readFromBlock(pBlock);

      // Is this the first pass?
      if (i == 0)
        {
          // Some additional checks required for the first pass
          if (left.getBlockUniqueId() == 0 && right.getBlockUniqueId() == 0
              && left.getSessionFirstBlockNumber() == 0
              && right.getSessionFirstBlockNumber() == 0)
            {
              // be sure we do not start at zero!
              blk_beg = blk_max; // start from last block
              id_beg = BEGINING_BLOCK_UNIQUE_ID;
              if (os.isDebug())
                {
                  OSDeviceDebug::putChar('d');
                }
              ready = true;

              break;
            }

          if (((right.getBlockUniqueId() >> 24) == 0xFF)
              && ((left.getBlockUniqueId() >> 24) == 0x00))
            {
              // ID rollover right at last block
              rollover = true;

              continue;
            }

          if (left.getBlockUniqueId() < right.getBlockUniqueId())
            {
              blk_beg = 0;
              id_beg = right.getBlockUniqueId() + 1;
              if (os.isDebug())
                {
                  OSDeviceDebug::putChar('i');
                }
              ready = true;

              break;
            }
        }
      else
        {
          // Second pass
          if ((signed long) left.getBlockUniqueId()
              < (signed long) right.getBlockUniqueId())
            {
              blk_beg = 0;
              id_beg = right.getBlockUniqueId() + 1;
              if (os.isDebug())
                {
                  OSDeviceDebug::putChar('I');
                }
              ready = true;

              break;
            }
        }

      for (; rightBlockNumber - leftBlockNumber > 1;)
        {
          if (i == 0)
            {
              if (((left.getBlockUniqueId() >> 24) == 0xFF)
                  && ((right.getBlockUniqueId() >> 24) == 0x00))
                {
                  // Rollover occured
                  rollover = true;
                  break;
                }
            }

          middleBlockNumber = (leftBlockNumber + rightBlockNumber) / 2;
          //r = readBlkHdr(pBuf, blk_k, &id_k, &begblk_k);
          r = readStorageBlock(middleBlockNumber, pBlock, 1);
          if (r != OSReturn::OS_OK)
            {
              if (os.isDebug())
                {
                  OSDeviceDebug::putChar('X');
                  OSDeviceDebug::putHex((unsigned short) r);
                }

              return r;
            }
          //id_k = Header::readBlockUniqueId(pBuf);
          //begblk_k = Header::readSessionFirstBlockNumber(pBuf);
          middle.readFromBlock(pBlock);

#if 1
          if (os.isDebug())
            {
              OSDeviceDebug::putNewLine();
              OSDeviceDebug::putHex(leftBlockNumber);
              OSDeviceDebug::putChar('-');
              OSDeviceDebug::putHex(left.getBlockUniqueId());
              OSDeviceDebug::putChar(' ');
              OSDeviceDebug::putHex(middleBlockNumber);
              OSDeviceDebug::putChar('-');
              OSDeviceDebug::putHex(middle.getBlockUniqueId());
              OSDeviceDebug::putChar(' ');
              OSDeviceDebug::putHex(rightBlockNumber);
              OSDeviceDebug::putChar('-');
              OSDeviceDebug::putHex(right.getBlockUniqueId());
            }
#endif

          if (i == 0)
            {
              if ((middle.getBlockUniqueId() < left.getBlockUniqueId())
                  || (middle.getBlockUniqueId() == 0))
                {
                  rightBlockNumber = middleBlockNumber;
                  right.setBlockUniqueId(middle.getBlockUniqueId());
                  right.setSessionFirstBlockNumber(
                      middle.getSessionFirstBlockNumber());
                }
              else
                {
                  leftBlockNumber = middleBlockNumber;
                  left.setBlockUniqueId(middle.getBlockUniqueId());
                  left.setSessionFirstBlockNumber(
                      middle.getSessionFirstBlockNumber());
                }
            }
          else
            {
              // Roll over case: use signed comparisons
              if (((signed long) middle.getBlockUniqueId()
                      < (signed long) left.getBlockUniqueId())
                  || (middle.getBlockUniqueId() == 0))
                {
                  rightBlockNumber = middleBlockNumber;
                  right.setBlockUniqueId(middle.getBlockUniqueId());
                  right.setSessionFirstBlockNumber(
                      middle.getSessionFirstBlockNumber());
                }
              else
                {
                  leftBlockNumber = middleBlockNumber;
                  left.setBlockUniqueId(middle.getBlockUniqueId());
                  left.setSessionFirstBlockNumber(
                      middle.getSessionFirstBlockNumber());
                }
            }
        }

      blk_beg = rightBlockNumber;
      id_beg = left.getBlockUniqueId() + 1;

      if (!rollover || i == 1)
        {
#if 0
          // Consistency test to prevent overwriting useful data
          // Warning: single session memory roll over case???


          if ( left.readSessionFirstBlockNumber() != right.readSessionFirstBlockNumber() )
            {
              ready = 1;
            }
          else
            {
              if ( os.isDebug() )
                {
                  clog << "#";
                }
            }

          // Warning: this test is not correct, after memory roll over
          // we might have same session begin

#else

          ready = true;

#endif

          break;
        }
    }

  id_last = id_beg;
  blk_crt = blk_beg;

  if (pid != 0)
  *pid = id_beg;

  if (plast != 0)
  *plast = blk_beg;

  return 0;

#endif
}

OSReturn_t
LargeCircularSessionsStorage::updateForwardReference(
    SessionBlockNumber_t sessionFirstBlockNumber,
    SessionBlockNumber_t nextSessionFirstBlockNumber, uint8_t* pBlock)
{
  // Update the current session to point to the next block
  OSReturn_t ret;
  ret = readStorageBlock(sessionFirstBlockNumber, pBlock, 1);
  if (ret == OSReturn::OS_OK)
    {
      Header::writeNextSessionFirstBlockNumber(nextSessionFirstBlockNumber,
          pBlock);
      ret = writeStorageBlock(sessionFirstBlockNumber, pBlock, 1);
    }
  return ret;

}

// ============================================================================

// ----- Constructors & destructors -------------------------------------------

LargeCircularSessionsStorage::Header::Header()
{
  OSDeviceDebug::putConstructor_P(PSTR("LargeCircularSessionsStorage::Header"),
      this);
}

LargeCircularSessionsStorage::Header::~Header()
{
  OSDeviceDebug::putDestructor_P(PSTR("LargeCircularSessionsStorage::Header"),
      this);
}

// ----- Public methods -------------------------------------------------------

LargeCircularSessionsStorage::Header&
LargeCircularSessionsStorage::Header::operator=(const Header& header)
{
  Header& h = ((Header&) header);

  // Copy all members from the parameter instance
  setMagic(h.getMagic());
  setBlockUniqueId(h.getBlockUniqueId());
  setSessionFirstBlockNumber(h.getSessionFirstBlockNumber());
  setSessionUniqueId(h.getSessionUniqueId());
  setNextSessionFirstBlockNumber(h.getNextSessionFirstBlockNumber());

  return *this;
}

void
LargeCircularSessionsStorage::Header::readFromBlock(const uint8_t* pBlock)
{
  setMagic(readMagic(pBlock));
  setSessionUniqueId(readSessionUniqueId(pBlock));
  setSessionFirstBlockNumber(readSessionFirstBlockNumber(pBlock));
  setBlockUniqueId(readBlockUniqueId(pBlock));
  setNextSessionFirstBlockNumber(readNextSessionFirstBlockNumber(pBlock));
}

void
LargeCircularSessionsStorage::Header::writeToBlock(uint8_t* pBlock)
{
  writeMagic(getMagic(), pBlock);
  writeSessionUniqueId(getSessionUniqueId(), pBlock);
  writeSessionFirstBlockNumber(getSessionFirstBlockNumber(), pBlock);
  writeBlockUniqueId(getBlockUniqueId(), pBlock);
  writeNextSessionFirstBlockNumber(getNextSessionFirstBlockNumber(), pBlock);

  fillUnusedSpace(pBlock);
}

void
LargeCircularSessionsStorage::Header::fillUnusedSpace(uint8_t* pHeader)
{
  uint8_t* p;
  p = pHeader + SIZE_OF_HEADER;

  for (size_t i = getSize() - SIZE_OF_HEADER; i > 0; --i)
    *p++ = UNUSED_SPACE_FILL_BYTE;

}
// ----- Static public methods ------------------------------------------------


// ============================================================================

// ----- Constructors & destructors -------------------------------------------

LargeCircularSessionsStorage::Writer::Writer(
    LargeCircularSessionsStorage& storage) :
  m_storage(storage)
{
  OSDeviceDebug::putConstructor_P(PSTR("LargeCircularSessionsStorage::Writer"),
      this);

  // Initialise the writer to search for the most recently written block
  m_currentHeader.setSessionUniqueId(0);
}

LargeCircularSessionsStorage::Writer::~Writer()
{
  OSDeviceDebug::putDestructor_P(PSTR("LargeCircularSessionsStorage::Writer"),
      this);
}

// ----- Public methods -------------------------------------------------------

OSReturn_t
LargeCircularSessionsStorage::Writer::createSession(
    SessionUniqueId_t sessionUniqueId)
{
  OSReturn_t ret;
  ret = OSReturn::OS_OK;

  // Initialise MAGIC
  m_currentHeader.setMagic(Header::MAGIC);

  if (m_currentHeader.getSessionUniqueId() == 0)
    {
      // This is the first call, we need to search the storage for the
      // most recently written block

      LargeCircularSessionsStorage::Header header;
      SessionBlockNumber_t mostRecentBlock;

      ret = getStorage().searchMostRecentlyWrittenBlock(m_block,
          &mostRecentBlock, header);
      if (ret == OSReturn::OS_NOT_FOUND)
        {
          // Initialise pointer on the first block
          m_currentBlockNumber = 0;
          m_currentHeader.setSessionFirstBlockNumber(m_currentBlockNumber);

          // Initialise block unique id
          m_currentHeader.setBlockUniqueId(1);

          if (sessionUniqueId != 0)
            {
              // Use the requested session unique id
              m_currentHeader.setSessionUniqueId(sessionUniqueId);
            }
          else
            {
              // If null, start with 1
              m_currentHeader.setSessionUniqueId(1);
            }

          ret = OSReturn::OS_OK;
        }
      else if (ret == OSReturn::OS_OK)
        {
          // Compute the next block
          m_currentBlockNumber
              = getStorage().computeCircularSessionBlockNumber(mostRecentBlock,
                  1);

          // Check if forward reference is updated
          if (header.getNextSessionFirstBlockNumber()
              == Header::DEFAULT_NEXTSESSIONFIRSTBLOCKNUMBER)
            {
              getStorage().updateForwardReference(
                  header.getSessionFirstBlockNumber(), m_currentBlockNumber,
                  m_block);
            }

          // Remember where the session starts
          m_currentHeader.setSessionFirstBlockNumber(m_currentBlockNumber);

          // The new block id is just next to the previous one
          m_currentHeader.setBlockUniqueId(header.getBlockUniqueId() + 1);

          if (sessionUniqueId != 0)
            {
              // Use the requested session unique id
              m_currentHeader.setSessionUniqueId(sessionUniqueId);
            }
          else
            {
              // If null, just increment the previous unique id
              m_currentHeader.setSessionUniqueId(
                  header.getSessionUniqueId() + 1);
            }
        }
      else
        {
          OSDeviceDebug::putString("searchMostRecentlyWrittenBlock() failed");
          OSDeviceDebug::putNewLine();

          return ret;
        }
    }
  else
    {
      // Subsequent sessions.

      // The forward reference should have been updated at closeSession().

      // The m_currentBlockNumber and the m_currentHeader header were already
      // updated by the previous writeSessionBlock()

      m_currentHeader.setSessionFirstBlockNumber(m_currentBlockNumber);

      if (sessionUniqueId != 0)
        {
          // Use the requested session unique id
          m_currentHeader.setSessionUniqueId(sessionUniqueId);
        }
      else
        {
          // If null, just increment the previous unique id
          m_currentHeader.incrementSessionUniqueId();
        }
    }

  OSDeviceDebug::putString("createSession() id=");
  OSDeviceDebug::putHex(m_currentHeader.getSessionUniqueId());
  OSDeviceDebug::putString(", blk id=");
  OSDeviceDebug::putHex(m_currentHeader.getBlockUniqueId());
  OSDeviceDebug::putString(", blk no=");
  OSDeviceDebug::putDec(m_currentHeader.getSessionFirstBlockNumber());
  OSDeviceDebug::putNewLine();

  return ret;
}

OSReturn_t
LargeCircularSessionsStorage::Writer::writeSessionBlock(uint8_t* pBlock)
{
  if (m_currentHeader.getSessionUniqueId() == 0)
    {
      return OSReturn::OS_NOT_INITIALISED;
    }

  // Prepare the current block header. All block have the forward reference
  // set to default, an illegal value.
  m_currentHeader.setNextSessionFirstBlockNumber(
      Header::DEFAULT_NEXTSESSIONFIRSTBLOCKNUMBER);

  // Update the block header
  m_currentHeader.writeToBlock(pBlock);

  OSReturn_t ret;

  // Write a full session block to the output device
  ret = getStorage().writeStorageBlock(m_currentBlockNumber, pBlock,
      getStorage().getSessionBlockSizeBlocks());

  // Increment the block number
  m_currentBlockNumber = getStorage().computeCircularSessionBlockNumber(
      m_currentBlockNumber, 1);

  // Increment the monotone increasing unique block number
  m_currentHeader.incrementBlockUniqueId();

  return ret;
}

OSReturn_t
LargeCircularSessionsStorage::Writer::closeSession(void)
{
  if (m_currentHeader.getSessionUniqueId() == 0)
    {
      return OSReturn::OS_NOT_INITIALISED;
    }

  // Update the header of the current session to point to the next (future)
  // session. This is to help forward navigation, since backward
  // navigation is already possible, all blocks point to the first
  // session block
  OSReturn_t ret;
  ret = getStorage().updateForwardReference(
      m_currentHeader.getSessionFirstBlockNumber(), m_currentBlockNumber,
      m_block);

  return ret;
}

// ============================================================================

// ----- Constructors & destructors -------------------------------------------

LargeCircularSessionsStorage::Reader::Reader(
    LargeCircularSessionsStorage& storage) :
  m_storage(storage)
{
  OSDeviceDebug::putConstructor_P(PSTR("LargeCircularSessionsStorage::Reader"),
      this);
}

LargeCircularSessionsStorage::Reader::~Reader()
{
  OSDeviceDebug::putDestructor_P(PSTR("LargeCircularSessionsStorage::Reader"),
      this);
}

// ----- Public methods -------------------------------------------------------

// Searches for the given session
OSReturn_t
LargeCircularSessionsStorage::Reader::openSession(SessionUniqueId_t sessionId)
{
  // TODO: implement
  sessionId = sessionId;

  return OSReturn::OS_OK;
}

// Without a session id, search for the most recent one
OSReturn_t
LargeCircularSessionsStorage::Reader::openMostRecentSession(void)
{
  OSReturn_t ret;

  ret = getStorage().searchMostRecentlyWrittenBlock(m_block,
      &m_mostRecentlyWrittenBlockNumber, m_currentHeader);
  if (ret == OSReturn::OS_NOT_FOUND)
    {
      return ret;
    }

  m_sessionLastBlockNumber = m_mostRecentlyWrittenBlockNumber;

  // TODO: should account for roll
  m_sessionLength = m_sessionLastBlockNumber
      - m_currentHeader.getSessionFirstBlockNumber() + 1;

  m_currentBlockNumber = m_currentHeader.getSessionFirstBlockNumber();

  OSDeviceDebug::putString("openMostRecentSession() id=");
  OSDeviceDebug::putHex(m_currentHeader.getSessionUniqueId());
  OSDeviceDebug::putString(", block=");
  OSDeviceDebug::putDec(m_currentHeader.getSessionFirstBlockNumber());
  OSDeviceDebug::putString("-");
  OSDeviceDebug::putDec(m_sessionLastBlockNumber);
  OSDeviceDebug::putNewLine();

  ret = OSReturn::OS_OK;

  return ret;
}

OSReturn_t
LargeCircularSessionsStorage::Reader::openOldestSession(void)
{
  OSReturn_t ret;

  ret = getStorage().searchMostRecentlyWrittenBlock(m_block,
      &m_mostRecentlyWrittenBlockNumber, m_currentHeader);
  if (ret == OSReturn::OS_NOT_FOUND)
    {
      // This means that no sessions were found
      return ret;
    }

  // Point to the most recently written block and walk backwords
  m_sessionLastBlockNumber = m_mostRecentlyWrittenBlockNumber;

  for (;;)
    {
      SessionBlockNumber_t lastBlockNumber;
      // Go to the block before this session's first block,
      // i.e. to the last block of the previous session
      lastBlockNumber = getStorage().computeCircularSessionBlockNumber(
          m_currentHeader.getSessionFirstBlockNumber(), -1);

      // Read the last block of the previous session
      ret = getStorage().readStorageBlock(lastBlockNumber, m_block, 1);
      if (ret != OSReturn::OS_OK)
        return ret;

      // We are now on the last block of the previous session

      // If not valid, quit loop, m_currentHeader points to the
      // oldest session
      if (Header::readMagic(m_block) != Header::MAGIC)
        break;

      SessionUniqueId_t lastBlockSessionUniqueId;
      lastBlockSessionUniqueId = Header::readSessionUniqueId(m_block);

      SessionBlockNumber_t firstBlockNumber;
      firstBlockNumber = Header::readSessionFirstBlockNumber(m_block);

      // Read the first block of the previous session
      ret = getStorage().readStorageBlock(firstBlockNumber, m_block, 1);
      if (ret != OSReturn::OS_OK)
        return ret;

      // We are now on the first block of the previous session
      if (Header::readMagic(m_block) != Header::MAGIC)
        break;

      SessionUniqueId_t firstBlockSessionUniqueId;
      firstBlockSessionUniqueId = Header::readSessionUniqueId(m_block);

      // Are we still inside the same session?
      if (firstBlockSessionUniqueId != lastBlockSessionUniqueId)
        break;

      m_sessionLastBlockNumber = lastBlockNumber;

      // Read current header from current session first block
      m_currentHeader.readFromBlock(m_block);
    }

  m_currentBlockNumber = m_currentHeader.getSessionFirstBlockNumber();

  // TODO: should account for roll
  m_sessionLength = m_sessionLastBlockNumber
      - m_currentHeader.getSessionFirstBlockNumber() + 1;

  OSDeviceDebug::putString("openOldestSession() id=");
  OSDeviceDebug::putHex(m_currentHeader.getSessionUniqueId());
  OSDeviceDebug::putString(", block=");
  OSDeviceDebug::putDec(m_currentHeader.getSessionFirstBlockNumber());
  OSDeviceDebug::putString("-");
  OSDeviceDebug::putDec(m_sessionLastBlockNumber);
  OSDeviceDebug::putNewLine();

  ret = OSReturn::OS_OK;

  return ret;
}

OSReturn_t
LargeCircularSessionsStorage::Reader::openPreviousSession(void)
{
  OSReturn_t ret;

  LargeCircularSessionsStorage::Header header;

  SessionBlockNumber_t lastBlockNumber;
  lastBlockNumber = getStorage().computeCircularSessionBlockNumber(
      m_sessionLastBlockNumber, -1);

  ret = getStorage().readStorageBlock(lastBlockNumber, m_block, 1);
  if (ret != OSReturn::OS_OK)
    return ret;

  // We are now on the last block of the previous session

  // If not valid, quit loop, the previous header points to the
  // oldest session
  if (Header::readMagic(m_block) != Header::MAGIC)
    return OSReturn::OS_END_OF_COLLECTION;

  SessionUniqueId_t lastBlockSessionUniqueId;
  lastBlockSessionUniqueId = Header::readSessionUniqueId(m_block);

  SessionBlockNumber_t firstBlockNumber;
  firstBlockNumber = Header::readSessionFirstBlockNumber(m_block);

  ret = getStorage().readStorageBlock(lastBlockNumber, m_block, 1);
  if (ret != OSReturn::OS_OK)
    return ret;

  // We are now on the first block of the previous session
  if (Header::readMagic(m_block) != Header::MAGIC)
    return OSReturn::OS_END_OF_COLLECTION;

  SessionUniqueId_t firstBlockSessionUniqueId;
  firstBlockSessionUniqueId = Header::readSessionUniqueId(m_block);

  if (firstBlockSessionUniqueId != lastBlockSessionUniqueId)
    return OSReturn::OS_END_OF_COLLECTION;

  m_currentHeader.setSessionUniqueId(firstBlockSessionUniqueId);
  m_currentHeader.setSessionFirstBlockNumber(firstBlockNumber);

  m_sessionLastBlockNumber = lastBlockNumber;

  // TODO: should account for roll
  m_sessionLength = m_sessionLastBlockNumber
      - m_currentHeader.getSessionFirstBlockNumber() + 1;

  m_currentBlockNumber = m_currentHeader.getSessionFirstBlockNumber();

  OSDeviceDebug::putString("openPreviousSession() id=");
  OSDeviceDebug::putHex(m_currentHeader.getSessionUniqueId());
  OSDeviceDebug::putString(", block=");
  OSDeviceDebug::putDec(m_currentHeader.getSessionFirstBlockNumber());
  OSDeviceDebug::putString("-");
  OSDeviceDebug::putDec(m_sessionLastBlockNumber);
  OSDeviceDebug::putNewLine();

  ret = OSReturn::OS_OK;

  return ret;
}

OSReturn_t
LargeCircularSessionsStorage::Reader::openNextSession(void)
{
  // Add blockNumber to session start
  m_currentBlockNumber = getStorage().computeCircularSessionBlockNumber(
      m_currentBlockNumber, 1);

  OSReturn_t ret;

  ret = getStorage().readStorageBlock(m_currentBlockNumber, m_block, 1);
  if (ret == OSReturn::OS_OK)
    {
      Magic_t magic;
      magic = LargeCircularSessionsStorage::Header::readMagic(m_block);

      // Check block magic
      if (magic == LargeCircularSessionsStorage::Header::MAGIC)
        {
          // If valid, read header from block
          m_currentHeader.readFromBlock(m_block);

          if (m_currentHeader.getNextSessionFirstBlockNumber()
              != Header::DEFAULT_NEXTSESSIONFIRSTBLOCKNUMBER)
            {
              m_sessionLastBlockNumber
                  = getStorage().computeCircularSessionBlockNumber(
                      m_currentHeader.getNextSessionFirstBlockNumber(), -1);

              // TODO: should account for roll
              m_sessionLength = m_sessionLastBlockNumber
                  - m_currentHeader.getSessionFirstBlockNumber() + 1;
            }
          else
            {
              m_sessionLength = 0;
            }
#if true
          OSDeviceDebug::putString("openNextSession() id=");
          OSDeviceDebug::putHex(m_currentHeader.getSessionUniqueId());
          OSDeviceDebug::putString(", block=");
          OSDeviceDebug::putDec(m_currentHeader.getSessionFirstBlockNumber());
          OSDeviceDebug::putString("-");
          if (m_sessionLength > 0)
            {
              OSDeviceDebug::putDec(m_sessionLastBlockNumber);
            }
          else
            {
              OSDeviceDebug::putChar('?');
            }
          OSDeviceDebug::putNewLine();
#endif
          ret = OSReturn::OS_OK;
        }
      else
        {
          OSDeviceDebug::putString("openNextSession() bad magic");
          OSDeviceDebug::putNewLine();

          ret = OSReturn::OS_END_OF_COLLECTION;
        }
    }

  return ret;
}

// Read the requested session block (minimum one device block)
// The block number should be between 0 and (sessionLength-1)
OSReturn_t
LargeCircularSessionsStorage::Reader::readSessionBlock(
    SessionBlockNumber_t blockNumber,
    OSDeviceBlock::BlockCount_t deviceBlocksCount, uint8_t* pBlock)
{
  if (m_sessionLength > 0)
    {
      // If session length is known, validate block number
      if (blockNumber >= m_sessionLength)
        {
          return OSReturn::OS_OUT_OF_RANGE;
        }
    }

  // Add blockNumber to session start
  m_currentBlockNumber = getStorage().computeCircularSessionBlockNumber(
      m_currentHeader.getSessionFirstBlockNumber(), (int) blockNumber);

#if false
  OSDeviceDebug::putString("readSessionBlock() blk=");
  OSDeviceDebug::putDec(blockNumber);
  OSDeviceDebug::putString(", storage blk=");
  OSDeviceDebug::putDec(m_currentBlockNumber);
  OSDeviceDebug::putNewLine();
#endif

  OSReturn_t ret;

  ret = getStorage().readStorageBlock(m_currentBlockNumber, pBlock,
      deviceBlocksCount);

  if (ret == OSReturn::OS_OK)
    {
      Magic_t magic;
      magic = LargeCircularSessionsStorage::Header::readMagic(pBlock);

      // Check block magic
      if (magic == LargeCircularSessionsStorage::Header::MAGIC)
        {
          // If valid, read block session unique id
          SessionUniqueId_t sessionUniqueId;
          sessionUniqueId
              = LargeCircularSessionsStorage::Header::readSessionUniqueId(
                  pBlock);

          if (sessionUniqueId != m_currentHeader.getSessionUniqueId())
            {
              // If the session id changes, return eof
              OSDeviceDebug::putString("readSessionBlock() eof");
              OSDeviceDebug::putNewLine();

              ret = OSReturn::OS_END_OF_COLLECTION;
            }

#if false
          OSDeviceDebug::putString("readSessionBlock() start block=");
          OSDeviceDebug::putDec(m_currentHeader.getSessionFirstBlockNumber());
          OSDeviceDebug::putString(", crt block=");
          OSDeviceDebug::putDec(m_currentBlockNumber);
          OSDeviceDebug::putNewLine();
#endif
          ret = OSReturn::OS_OK;
        }
      else
        {
          OSDeviceDebug::putString("readSessionBlock() bad magic");
          OSDeviceDebug::putNewLine();

          ret = OSReturn::OS_END_OF_COLLECTION;
        }
    }

  return ret;
}

#if false
// Does not work yet
OSReturn_t
LargeCircularSessionsStorage::Reader::readNextSessionBlock(
    OSDeviceBlock::BlockCount_t deviceBlocksCount, uint8_t* pBlock)
  {
    OSReturn_t ret;

    ret = getStorage().readStorageBlock(m_currentBlockNumber, pBlock,
        deviceBlocksCount);

    if (ret == OSReturn::OS_OK)
      {
        Magic_t magic;
        magic = LargeCircularSessionsStorage::Header::readMagic(pBlock);

        // Check block magic
        if (magic == LargeCircularSessionsStorage::Header::MAGIC)
          {
            // If valid, read block session unique id
            SessionUniqueId_t sessionUniqueId;
            sessionUniqueId
            = LargeCircularSessionsStorage::Header::readSessionUniqueId(
                pBlock);

            if (sessionUniqueId != m_currentHeader.getSessionUniqueId())
              {
                // If the session id changes, return eof
                OSDeviceDebug::putString("readNextSessionBlock() eof");
                OSDeviceDebug::putNewLine();

                ret = OSReturn::OS_END_OF_COLLECTION;
              }

#if true
            OSDeviceDebug::putString("readNextSessionBlock() start block=");
            OSDeviceDebug::putDec(m_currentHeader.getSessionFirstBlockNumber());
            OSDeviceDebug::putString(", crt block=");
            OSDeviceDebug::putDec(m_currentBlockNumber);
            OSDeviceDebug::putNewLine();
#endif
            // Add blockNumber to session start
            m_currentBlockNumber = getStorage().computeCircularSessionBlockNumber(
                m_currentBlockNumber, 1);

            ret = OSReturn::OS_OK;
          }
        else
          {
            OSDeviceDebug::putString("readNextSessionBlock() bad magic");
            OSDeviceDebug::putNewLine();

            ret = OSReturn::OS_END_OF_COLLECTION;
          }
      }

    return ret;
  }
#endif

OSReturn_t
LargeCircularSessionsStorage::Reader::closeSession(void)
{
  return OSReturn::OS_OK;
}

// ----------------------------------------------------------------------------

// ============================================================================

#endif /* defined(OS_INCLUDE_LARGECIRCULARSESSIONSSTORAGE) */
