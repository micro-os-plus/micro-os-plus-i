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

  m_event = (OSEvent_t) (&m_event);
}

LargeCircularSessionsStorage::~LargeCircularSessionsStorage()
{
  OSDeviceDebug::putDestructor_P(PSTR("LargeCircularSessionsStorage"), this);
}

// ----- Public methods -------------------------------------------------------

LargeCircularSessionsStorage::SessionBlockNumber_t
LargeCircularSessionsStorage::getStorageSizeSessionBlocks(void)
{
  // Compute the number of session blocks on the device size and the block size
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

  // Move the block pointer forward/backward in the circular storage
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

LargeCircularSessionsStorage::SessionBlockNumber_t
LargeCircularSessionsStorage::computeCircularSessionLength(
    SessionBlockNumber_t last, SessionBlockNumber_t first)
{
  // The usual case is when both numbers are in short succession
  if (last >= first)
    return last - first + 1;

  // In case we're just rolling over the circular border,
  // use storage size.

  // The trick here is that the session length is significantly smaller
  // than the storage size.

  SessionBlockNumber_t size;
  size = getStorageSizeSessionBlocks();

  return last + size - first + 1;
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

  // Compute the device address and call the device read
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

  // Compute the device address and call the device write
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

  if (!left.isMagicValid())
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

  if (!right.isMagicValid())
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
              // Found, the 'left' header is the most recent
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

          if (!middle.isMagicValid())
            {
              // Middle header invalid, consider it as last written
              rightBlockNumber = middleBlockNumber;
              right = middle;
            }
          else
            {
              // Middle in use, adjust first
              leftBlockNumber = middleBlockNumber;
              left = middle;
            }
        }

      // Found, the left block is the most recently written
      // The right is the first empty block, with invalid magic

      *pMostRecentBlock = leftBlockNumber;
      header = left;
      return OSReturn::OS_OK;
    }

  // Check if the discontinuity is exactly at the circular border
  if (right.getBlockUniqueId() + 1 != left.getBlockUniqueId())
    {
      // Found, the right block (the last in the array) is the most recently
      // written. The left block (the first in the array) is the oldest
      // block, with the oldest block unique id, that we'll overwrite

      *pMostRecentBlock = rightBlockNumber;
      header = right;
      return OSReturn::OS_OK;
    }

  // After checking the border, the circular storage is now simplified to a
  // linear storage, so a binary search, similar to the above, would do
  // the job

  for (;;)
    {
      OSDeviceDebug::putString(" search ");
      OSDeviceDebug::putDec(leftBlockNumber);
      OSDeviceDebug::putChar('-');
      OSDeviceDebug::putDec(rightBlockNumber);
      OSDeviceDebug::putNewLine();

      if (leftBlockNumber + 1 == rightBlockNumber)
        {
          // Found, the 'left' header is the most recent
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

      // There are two tricks below:
      // - the number of blocks in the storage, and, since the block
      // unique ID is monotone increasing with step 1, the range of existing
      // unique IDs in the storage at a given moment, is lower than the
      // range of the variables used for the block unique ID. In other words,
      // at a given time we have IDs from N to N+size, while the variables
      // can range from 0 to 0xFF..FF.
      // - we count that the unique IDs are unsigned values, so when we
      // subtract something from a lower value we get a large result,
      // like 0 - 1 = 0xFFFF

      // If the size is much lower than 0xFF..FF, the unsigned difference
      // between the unique block IDs in the half where the discontinuity
      // is present must be higher than the difference where the IDs are
      // monotone

      if ((middle.getBlockUniqueId() - left.getBlockUniqueId())
          > (right.getBlockUniqueId() - middle.getBlockUniqueId()))
        {
          // The discontinuity is in the left half
          rightBlockNumber = middleBlockNumber;
          right = middle;
        }
      else
        {
          // The discontinuity is in the right half
          leftBlockNumber = middleBlockNumber;
          left = middle;
        }
    }

  // Found, the left block is the most recently written.
  // The right is the oldest written block, that we'll overwrite

  *pMostRecentBlock = leftBlockNumber;
  header = left;
  return OSReturn::OS_OK;

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

  m_magic = 0;
  m_sessionUniqueId = 0;
  m_sessionFirstBlockNumber = 0;
  m_blockUniqueId = 0;
  m_nextSessionFirstBlockNumber = Header::DEFAULT_NEXTSESSIONFIRSTBLOCKNUMBER;

  m_currentBlockNumber = 0;
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

LargeCircularSessionsStorage::Session::Session()
{
  OSDeviceDebug::putConstructor_P(
      PSTR("LargeCircularSessionsStorage::Session"), this);

  m_sessionUniqueId = 0;
  m_sessionFirstBlockNumber = 0;
  m_sessionFirstBlockNumber = Header::DEFAULT_NEXTSESSIONFIRSTBLOCKNUMBER;
  m_sessionLength = 0;
  m_nextSessionFirstBlockNumber = Header::DEFAULT_NEXTSESSIONFIRSTBLOCKNUMBER;
}

LargeCircularSessionsStorage::Session::~Session()
{
  OSDeviceDebug::putDestructor_P(PSTR("LargeCircularSessionsStorage::Session"),
      this);
}

// ============================================================================

// ----- Constructors & destructors -------------------------------------------

LargeCircularSessionsStorage::SessionBlock::SessionBlock()
{
  OSDeviceDebug::putConstructor_P(
      PSTR("LargeCircularSessionsStorage::SessionBlock"), this);

  m_blockUniqueId = 0;
  m_blockNumber = 0;
}

LargeCircularSessionsStorage::SessionBlock::~SessionBlock()
{
  OSDeviceDebug::putDestructor_P(
      PSTR("LargeCircularSessionsStorage::SessionBlock"), this);
}

// ============================================================================

// ----- Constructors & destructors -------------------------------------------

LargeCircularSessionsStorage::Writer::Writer(
    LargeCircularSessionsStorage& storage) :
  m_storage(storage)
{
  OSDeviceDebug::putConstructor_P(PSTR("LargeCircularSessionsStorage::Writer"),
      this);

  m_pEventNotifier = NULL;

  // Initialise the writer to search for the most recently written block
  m_currentHeader.setSessionUniqueId(0);
}

LargeCircularSessionsStorage::Writer::Writer(
    LargeCircularSessionsStorage& storage, OSEventNotifier* pEventNotifier) :
  m_storage(storage)
{
  OSDeviceDebug::putConstructor_P(PSTR("LargeCircularSessionsStorage::Writer"),
      this);

  m_pEventNotifier = pEventNotifier;

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

  LargeCircularSessionsStorage::Header currentHeader;

  if (!getStorage().getMostRecentlyWrittenBlockHeader().isMagicValid())
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
          currentHeader.setCurrentBlockNumber(0);
          currentHeader.setSessionFirstBlockNumber(
              currentHeader.getCurrentBlockNumber());

          // Initialise block unique id
          currentHeader.setBlockUniqueId(1);

          if (sessionUniqueId != 0)
            {
              // Use the requested session unique id
              currentHeader.setSessionUniqueId(sessionUniqueId);
            }
          else
            {
              // If null, start with 1
              currentHeader.setSessionUniqueId(1);
            }

          ret = OSReturn::OS_OK;
        }
      else if (ret == OSReturn::OS_OK)
        {
          // Compute the next block
          currentHeader.setCurrentBlockNumber(
              getStorage().computeCircularSessionBlockNumber(mostRecentBlock, 1));

          // Check if forward reference is updated
          if (header.getNextSessionFirstBlockNumber()
              == Header::DEFAULT_NEXTSESSIONFIRSTBLOCKNUMBER)
            {
              getStorage().updateForwardReference(
                  header.getSessionFirstBlockNumber(),
                  currentHeader.getCurrentBlockNumber(), m_block);
            }

          // Remember where the session starts
          currentHeader.setSessionFirstBlockNumber(
              currentHeader.getCurrentBlockNumber());

          // The new block id is just next to the previous one
          currentHeader.setBlockUniqueId(header.getBlockUniqueId() + 1);

          if (sessionUniqueId != 0)
            {
              // Use the requested session unique id
              currentHeader.setSessionUniqueId(sessionUniqueId);
            }
          else
            {
              // If null, just increment the previous unique id
              currentHeader.setSessionUniqueId(header.getSessionUniqueId() + 1);
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

      currentHeader.setSessionFirstBlockNumber(
          currentHeader.getCurrentBlockNumber());

      if (sessionUniqueId != 0)
        {
          // Use the requested session unique id
          currentHeader.setSessionUniqueId(sessionUniqueId);
        }
      else
        {
          // If null, just increment the previous unique id
          currentHeader.incrementSessionUniqueId();
        }
    }

  OSDeviceDebug::putString("createSession() id=");
  OSDeviceDebug::putHex(currentHeader.getSessionUniqueId());
  OSDeviceDebug::putString(", blk id=");
  OSDeviceDebug::putHex(currentHeader.getBlockUniqueId());
  OSDeviceDebug::putString(", blk no=");
  OSDeviceDebug::putDec(currentHeader.getSessionFirstBlockNumber());
  OSDeviceDebug::putNewLine();

  os.sched.lock.enter();
    {
      // Minimal protection, in case we are running in parallel with a
      // reader session.
      if (!getStorage().getMostRecentlyWrittenBlockHeader().isMagicValid())
        {
          // Header is complete now, set MAGIC
          currentHeader.setMagic(Header::MAGIC);

          // Copy local header to parent class, to be used by reader too
          getStorage().getMostRecentlyWrittenBlockHeader() = currentHeader;
        }
    }
  os.sched.lock.exit();

  // Inform readers that we are now in a writing session
  getStorage().setIsWriting(true);

  // Inform readers that a new session was created
  notifyReaders();

  return ret;
}

OSReturn_t
LargeCircularSessionsStorage::Writer::writeSessionBlock(uint8_t* pBlock)
{
  Header& currentHeader = getStorage().getMostRecentlyWrittenBlockHeader();

  if (!currentHeader.isMagicValid())
    {
      return OSReturn::OS_NOT_INITIALISED;
    }

  // Prepare the current block header. All new blocks have the forward
  // reference set to default, an illegal value.
  currentHeader.setNextSessionFirstBlockNumber(
      Header::DEFAULT_NEXTSESSIONFIRSTBLOCKNUMBER);

  // Update the block header
  currentHeader.writeToBlock(pBlock);

  OSReturn_t ret;

  // Write a full session block to the output device
  ret = getStorage().writeStorageBlock(currentHeader.getCurrentBlockNumber(),
      pBlock, getStorage().getSessionBlockSizeBlocks());

  // Increment the block number
  currentHeader.setCurrentBlockNumber(
      getStorage().computeCircularSessionBlockNumber(
          currentHeader.getCurrentBlockNumber(), 1));

  // Increment the monotone increasing unique block number
  currentHeader.incrementBlockUniqueId();

  // Inform readers that a new block was added
  notifyReaders();

  return ret;
}

OSReturn_t
LargeCircularSessionsStorage::Writer::closeSession(void)
{
  Header& currentHeader = getStorage().getMostRecentlyWrittenBlockHeader();

  if (!currentHeader.isMagicValid())
    {
      return OSReturn::OS_NOT_INITIALISED;
    }

  // Update the header of the current session to point to the next (future)
  // session. This is to help forward navigation, since backward
  // navigation is already possible, all blocks point to the first
  // session block
  OSReturn_t ret;
  ret = getStorage().updateForwardReference(
      currentHeader.getSessionFirstBlockNumber(),
      currentHeader.getCurrentBlockNumber(), m_block);

  // Update pointer to next block to allow the reader to
  // compute the session length
  currentHeader.setNextSessionFirstBlockNumber(
      currentHeader.getCurrentBlockNumber());

  // Inform readers that we are no longer in a writing session
  getStorage().setIsWriting(false);

  // Inform readers that the session was closed
  notifyReaders();

  return ret;
}

void
LargeCircularSessionsStorage::Writer::notifyReaders(void)
{
  if (m_pEventNotifier != NULL)
    {
      OSEvent_t event;
      event = getStorage().getEvent();

      m_pEventNotifier->eventNotify(event, (OSEventWaitReturn_t) event);
    }
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

// Searches for the given session.
// Special cases are:
//  OLDEST_SESSSION_UNIQUE_ID - open the oldest session available
//  NEWEST_SESSSION_UNIQUE_ID - open the most recently written session

OSReturn_t
LargeCircularSessionsStorage::Reader::openSession(SessionUniqueId_t sessionId,
    bool doNotBlock)
{
  OSReturn_t ret;
  SessionUniqueId_t mostRecentSessionUniqueId;
  SessionUniqueId_t currentSessionUniqueId;

  ret = openMostRecentSession();
  if (ret != OSReturn::OS_OK)
    {
      return ret;
    }

  if (sessionId == NEWEST_SESSSION_UNIQUE_ID)
    return OSReturn::OS_OK;

  mostRecentSessionUniqueId = m_currentHeader.getSessionUniqueId();

  // Did we ask for exactly the most recently written session?
  if (mostRecentSessionUniqueId == sessionId)
    {
      // Fine, we're done
      return OSReturn::OS_OK;
    }

  // Did we ask for a future session id?
  if (sessionId > mostRecentSessionUniqueId)
    {
      if (doNotBlock)
        return OSReturn::OS_WOULD_BLOCK;

      for (;;)
        {
          // Wait for the requested session to become available
          // For this we wake-up every time the writer updates the storage

          ret = os.sched.eventWait(getStorage().getEvent());
          if (ret == OSReturn::OS_CANCELLED)
            {
              return ret;
            }

          // Copy header prepared by writer
          m_currentHeader = getStorage().getMostRecentlyWrittenBlockHeader();

          if (m_currentHeader.getSessionUniqueId() == sessionId)
            {
              // TODO: check if other things are needed
              return OSReturn::OS_OK;
            }
        }
    }

  // Search for the given session id
  do
    {
      ret = openPreviousSession();
      if (ret != OSReturn::OS_OK)
        {
          return ret;
        }

      currentSessionUniqueId = m_currentHeader.getSessionUniqueId();

      if (currentSessionUniqueId == sessionId)
        {
          return OSReturn::OS_OK;
        }
    }
  while (currentSessionUniqueId != mostRecentSessionUniqueId);

  if (sessionId == OLDEST_SESSSION_UNIQUE_ID)
    return OSReturn::OS_OK;

  // Otherwise inform the caller we could not find the given session id
  return OSReturn::OS_ITEM_NOT_FOUND;
}

// Without a session id, search for the most recent one
OSReturn_t
LargeCircularSessionsStorage::Reader::openMostRecentSession(void)
{
  OSReturn_t ret;
  ret = OSReturn::OS_OK;

  if (!getStorage().getMostRecentlyWrittenBlockHeader().isMagicValid())
    {
      // If there was now prior write, we need to search
      // The result is in m_currentHeader
      ret = getStorage().searchMostRecentlyWrittenBlock(m_block,
          &m_mostRecentlyWrittenBlockNumber, m_currentHeader);
      // TODO: change to != OS_OK
      if (ret == OSReturn::OS_NOT_FOUND)
        {
          return ret;
        }

      os.sched.lock.enter();
        {
          // Minimal protection, in case we are running in parallel with a
          // writer session.
          if (!getStorage().getMostRecentlyWrittenBlockHeader().isMagicValid())
            {
              // Copy local header to parent class, to be used by writer too
              getStorage().getMostRecentlyWrittenBlockHeader()
                  = m_currentHeader;
            }
        }
      os.sched.lock.exit();
    }
  else
    {
      m_currentHeader = getStorage().getMostRecentlyWrittenBlockHeader();
    }

  // Now we know the header of the most recently written block

  m_sessionLastBlockNumber = m_mostRecentlyWrittenBlockNumber;
  m_sessionFirstBlockNumber = m_currentHeader.getSessionFirstBlockNumber();

  m_sessionLength = getStorage().computeCircularSessionLength(
      m_sessionLastBlockNumber, m_currentHeader.getSessionFirstBlockNumber());

  m_currentHeader.setSessionFirstBlockNumber(
      m_currentHeader.getSessionFirstBlockNumber());

  OSDeviceDebug::putString("openMostRecentSession() id=");
  OSDeviceDebug::putHex(m_currentHeader.getSessionUniqueId());
  OSDeviceDebug::putString(", block=");
  OSDeviceDebug::putDec(m_sessionFirstBlockNumber);
  OSDeviceDebug::putString("-");
  OSDeviceDebug::putDec(m_sessionLastBlockNumber);
  OSDeviceDebug::putNewLine();

  ret = OSReturn::OS_OK;

  return ret;
}

#if false

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
    m_sessionFirstBlockNumber = m_currentHeader.getSessionFirstBlockNumber();

    m_sessionLength = getStorage().computeCircularSessionLength(
        m_sessionLastBlockNumber, m_currentHeader.getSessionFirstBlockNumber());

    OSDeviceDebug::putString("openOldestSession() id=");
    OSDeviceDebug::putHex(m_currentHeader.getSessionUniqueId());
    OSDeviceDebug::putString(", block=");
    OSDeviceDebug::putDec(m_sessionFirstBlockNumber);
    OSDeviceDebug::putString("-");
    OSDeviceDebug::putDec(m_sessionLastBlockNumber);
    OSDeviceDebug::putNewLine();

    ret = OSReturn::OS_OK;

    return ret;
  }
#endif

OSReturn_t
LargeCircularSessionsStorage::Reader::openPreviousSession(void)
{
  OSReturn_t ret;

  LargeCircularSessionsStorage::Header header;

  // From the first block of the current session, go one block before,
  // i.e. the last block of the previous session
  SessionBlockNumber_t lastBlockNumber;
  lastBlockNumber = getStorage().computeCircularSessionBlockNumber(
      m_sessionFirstBlockNumber, -1);

  // Read the last block of the previous session
  ret = getStorage().readStorageBlock(lastBlockNumber, m_block, 1);
  if (ret != OSReturn::OS_OK)
    return ret;

  // We are now on the last block of the previous session

  // If not valid, return, the previous header points to the
  // oldest session
  if (Header::readMagic(m_block) != Header::MAGIC)
    return OSReturn::OS_END_OF_COLLECTION;

  SessionUniqueId_t lastBlockSessionUniqueId;
  lastBlockSessionUniqueId = Header::readSessionUniqueId(m_block);

  SessionBlockNumber_t sessionFirstBlockNumber;
  sessionFirstBlockNumber = Header::readSessionFirstBlockNumber(m_block);

  ret = getStorage().readStorageBlock(lastBlockNumber, m_block, 1);
  if (ret != OSReturn::OS_OK)
    return ret;

  // We are now on the first block of the previous session
  // If not valid, return
  if (Header::readMagic(m_block) != Header::MAGIC)
    return OSReturn::OS_END_OF_COLLECTION;

  SessionUniqueId_t firstBlockSessionUniqueId;
  firstBlockSessionUniqueId = Header::readSessionUniqueId(m_block);

  if (firstBlockSessionUniqueId != lastBlockSessionUniqueId)
    return OSReturn::OS_END_OF_COLLECTION;

  m_currentHeader.setSessionUniqueId(firstBlockSessionUniqueId);
  m_currentHeader.setSessionFirstBlockNumber(sessionFirstBlockNumber);

  m_sessionLastBlockNumber = lastBlockNumber;

  m_sessionLength = getStorage().computeCircularSessionLength(
      m_sessionLastBlockNumber, m_currentHeader.getSessionFirstBlockNumber());

  //m_currentBlockNumber = m_currentHeader.getSessionFirstBlockNumber();
  m_sessionFirstBlockNumber = m_currentHeader.getSessionFirstBlockNumber();

  OSDeviceDebug::putString("openPreviousSession() id=");
  OSDeviceDebug::putHex(m_currentHeader.getSessionUniqueId());
  OSDeviceDebug::putString(", block=");
  OSDeviceDebug::putDec(m_sessionFirstBlockNumber);
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
  m_currentHeader.setSessionFirstBlockNumber(
      getStorage().computeCircularSessionBlockNumber(
          m_currentHeader.getCurrentBlockNumber(), 1));

  OSReturn_t ret;

  ret = getStorage().readStorageBlock(m_currentHeader.getCurrentBlockNumber(),
      m_block, 1);
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

              m_sessionLength = getStorage().computeCircularSessionLength(
                  m_sessionLastBlockNumber,
                  m_currentHeader.getSessionFirstBlockNumber());
            }
          else
            {
              m_sessionLength = 0;
            }

          m_sessionFirstBlockNumber
              = m_currentHeader.getSessionFirstBlockNumber();

#if true
          OSDeviceDebug::putString("openNextSession() id=");
          OSDeviceDebug::putHex(m_currentHeader.getSessionUniqueId());
          OSDeviceDebug::putString(", block=");
          OSDeviceDebug::putDec(m_sessionFirstBlockNumber);
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
    OSDeviceBlock::BlockCount_t deviceBlocksCount, uint8_t* pBlock,
    bool doNotBlock)
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
  m_currentHeader.setSessionFirstBlockNumber(
      getStorage().computeCircularSessionBlockNumber(
          m_currentHeader.getSessionFirstBlockNumber(), (int) blockNumber));

#if false
  OSDeviceDebug::putString("readSessionBlock() blk=");
  OSDeviceDebug::putDec(blockNumber);
  OSDeviceDebug::putString(", storage blk=");
  OSDeviceDebug::putDec(m_currentBlockNumber);
  OSDeviceDebug::putNewLine();
#endif

  OSReturn_t ret;

  for (;;)
    {
      ret = getStorage().readStorageBlock(
          m_currentHeader.getCurrentBlockNumber(), pBlock, deviceBlocksCount);

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
              OSDeviceDebug::putDec(currentHeader.getSessionFirstBlockNumber());
              OSDeviceDebug::putString(", crt block=");
              OSDeviceDebug::putDec(m_currentBlockNumber);
              OSDeviceDebug::putNewLine();
#endif
              ret = OSReturn::OS_OK;

              break;
            }
          else
            {
              OSDeviceDebug::putString("readSessionBlock() bad magic");
              OSDeviceDebug::putNewLine();

              ret = OSReturn::OS_END_OF_COLLECTION;
            }

          if (ret != OSReturn::OS_END_OF_COLLECTION)
            {
              break;
            }

          if (doNotBlock)
            {
              break;
            }
        }
      // Read error case
      break;
    }

  return ret;
}

OSReturn_t
LargeCircularSessionsStorage::Reader::closeSession(void)
{
  return OSReturn::OS_OK;
}

// ----------------------------------------------------------------------------

// ============================================================================

#endif /* defined(OS_INCLUDE_LARGECIRCULARSESSIONSSTORAGE) */
