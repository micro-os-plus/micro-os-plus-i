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

  OSDeviceDebug::putString("size=");
  OSDeviceDebug::putDec(size);
  OSDeviceDebug::putNewLine();

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
  if (deviceBlocksCount > getSessionBlockSizeBlocks())
    return OSReturn::OS_BAD_PARAMETER;

  return getDevice().readBlocks(blockNumber * getSessionBlockSizeBlocks(),
      pSessionBlock, deviceBlocksCount);
}

OSReturn_t
LargeCircularSessionsStorage::writeStorageBlock(
    SessionBlockNumber_t blockNumber, uint8_t* pSessionBlock,
    OSDeviceBlock::BlockCount_t deviceBlocksCount)
{
  if (deviceBlocksCount > getSessionBlockSizeBlocks())
    return OSReturn::OS_BAD_PARAMETER;

  return getDevice().writeBlocks(blockNumber * getSessionBlockSizeBlocks(),
      pSessionBlock, deviceBlocksCount);
}

OSReturn_t
LargeCircularSessionsStorage::searchMostRecentlyWrittenBlock(
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

  uint8_t* pBuf;
  pBuf = m_ibuf;

  OSReturn_t ret;

  ret = readStorageBlock(leftBlockNumber, pBuf, 1);
  if (ret != OSReturn::OS_OK)
    {
      OSDeviceDebug::putChar('V');
      OSDeviceDebug::putHex((unsigned short) ret);
      return ret;
    }
  left.readFromHeader(pBuf);

  if (left.getMagic() != Header::MAGIC)
    {
      // If the first block does not have a magic, then we consider
      // the entire partition to be empty.
      return OSReturn::OS_NOT_FOUND;
    }

  ret = readStorageBlock(rightBlockNumber, pBuf, 1);
  if (ret != OSReturn::OS_OK)
    {
      OSDeviceDebug::putChar('W');
      OSDeviceDebug::putHex((unsigned short) ret);
      return ret;
    }
  right.readFromHeader(pBuf);

  if (right.getMagic() != Header::MAGIC)
    {
      // If the last block does not have a magic, then we consider
      // the entire partition to be linear, without roll, so we just
      // do a binary search of the last block with valid magic

      for (;;)
        {
          OSDeviceDebug::putString("search ");
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
          ret = readStorageBlock(middleBlockNumber, pBuf, 1);
          if (ret != OSReturn::OS_OK)
            {
              OSDeviceDebug::putChar('X');
              OSDeviceDebug::putHex((unsigned short) ret);
              return ret;
            }
          middle.readFromHeader(pBuf);

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

  uint8_t* pBuf;
  pBuf = m_ibuf;

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
      r = readStorageBlock(leftBlockNumber, pBuf, 1);
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
      left.readFromHeader(pBuf);

      //r = readBlkHdr(pBuf, blk_j, &id_j, &begblk_j);
      r = readStorageBlock(rightBlockNumber, pBuf, 1);
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
      right.readFromHeader(pBuf);

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
              id_beg = BEGINING_UNIQUE_ID;
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
          r = readStorageBlock(middleBlockNumber, pBuf, 1);
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
          middle.readFromHeader(pBuf);

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
  setMagic(h.getMagic());
  setBlockUniqueId(h.getBlockUniqueId());
  setSessionFirstBlockNumber(h.getSessionFirstBlockNumber());
  setSessionUniqueId(h.getSessionUniqueId());

  return *this;
}

void
LargeCircularSessionsStorage::Header::readFromHeader(const uint8_t* pHeader)
{
  setMagic(readMagic(pHeader));
  setSessionUniqueId(readSessionUniqueId(pHeader));
  setSessionFirstBlockNumber(readSessionFirstBlockNumber(pHeader));
  setBlockUniqueId(readBlockUniqueId(pHeader));
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

  if (m_sessionUniqueId == 0)
    {
      // This is the first call, we need to search the storage for the
      // most recently written block

      LargeCircularSessionsStorage::Header header;
      SessionBlockNumber_t mostRecentBlock;

      ret = getStorage().searchMostRecentlyWrittenBlock(&mostRecentBlock,
          header);
      if (ret == OSReturn::OS_NOT_FOUND)
        {
          // Initialise pointer on the first block
          m_currentBlockNumber = 0;
          m_sessionFirstBlockNumber = m_currentBlockNumber;

          // Initialise block unique id
          m_blockUniqueId = 1;

          if (sessionUniqueId != 0)
            {
              // Use the requested session unique id
              m_sessionUniqueId = sessionUniqueId;
            }
          else
            {
              // If null, start with 1
              m_sessionUniqueId = 1;
            }
        }
      else if (ret == OSReturn::OS_OK)
        {
          // Compute the next block
          m_currentBlockNumber
              = getStorage().computeCircularSessionBlockNumber(mostRecentBlock,
                  1);

          // Remember where the session starts
          m_sessionFirstBlockNumber = m_currentBlockNumber;

          // The new block id is just next to the previous one
          m_blockUniqueId = header.getBlockUniqueId() + 1;

          if (sessionUniqueId != 0)
            {
              // Use the requested session unique id
              m_sessionUniqueId = sessionUniqueId;
            }
          else
            {
              // If null, just increment the previous unique id
              m_sessionUniqueId = header.getSessionUniqueId() + 1;
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
      // Subsequent sessions

      // The m_currentBlockNumber and the m_blockUniqueId were already
      // incremented by the previous writeSessionBlock()

      m_sessionFirstBlockNumber = m_currentBlockNumber;

      if (sessionUniqueId != 0)
        {
          // Use the requested session unique id
          m_sessionUniqueId = sessionUniqueId;
        }
      else
        {
          // If null, just increment the previous unique id
          m_sessionUniqueId++;
        }
    }

  OSDeviceDebug::putString("createSession id=");
  OSDeviceDebug::putHex(m_sessionUniqueId);
  OSDeviceDebug::putString(", blk id=");
  OSDeviceDebug::putHex(m_blockUniqueId);
  OSDeviceDebug::putString(", blk no=");
  OSDeviceDebug::putDec(m_sessionFirstBlockNumber);
  OSDeviceDebug::putNewLine();

  return ret;
}

OSReturn_t
LargeCircularSessionsStorage::Writer::closeSession(void)
{
  ; // Currently nothing to do

  return OSReturn::OS_OK;
}

OSReturn_t
LargeCircularSessionsStorage::Writer::writeSessionBlock(uint8_t* pBlock)
{
  if (m_sessionUniqueId == 0)
    {
      return OSReturn::OS_NOT_INITIALISED;
    }

  OSDeviceBlock::BlockSize_t nBlocks;
  nBlocks = getStorage().getSessionBlockSizeBlocks();

  // Fill-in the block header, currently having these fields
  //    magic
  //    sessionUniqueId
  //    sessionFirstBlockNumber
  //    blockUniqueId

  LargeCircularSessionsStorage::Header::writeMagic(
      LargeCircularSessionsStorage::Header::MAGIC, pBlock);
  LargeCircularSessionsStorage::Header::writeSessionUniqueId(m_sessionUniqueId,
      pBlock);
  LargeCircularSessionsStorage::Header::writeSessionFirstBlockNumber(
      m_sessionFirstBlockNumber, pBlock);
  LargeCircularSessionsStorage::Header::writeBlockUniqueId(m_blockUniqueId,
      pBlock);

  OSReturn_t ret;

  // Write a full session block to the output device
  ret = getStorage().writeStorageBlock(m_currentBlockNumber, pBlock, nBlocks);

  // Increment the block number; when reaching storage size, roll over
  ++m_currentBlockNumber;
  if (m_currentBlockNumber >= getStorage().getStorageSizeSessionBlocks())
    {
      m_currentBlockNumber = 0;
    }

  // Increment the monotone increasing unique block number
  ++m_blockUniqueId;

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

  LargeCircularSessionsStorage::Header header;

  ret = getStorage().searchMostRecentlyWrittenBlock(&m_mostRecentlyWrittenBlockNumber,
      header);
  if (ret == OSReturn::OS_NOT_FOUND)
    {
      return ret;
    }

  m_sessionLastBlockNumber = m_mostRecentlyWrittenBlockNumber;

  m_sessionUniqueId = header.getSessionUniqueId();
  m_sessionFirstBlockNumber = header.getSessionFirstBlockNumber();

  // TODO: should account for roll
  m_sessionLength = m_sessionLastBlockNumber - m_sessionFirstBlockNumber + 1;

  m_currentBlockNumber = m_sessionFirstBlockNumber;

  OSDeviceDebug::putString("openMostRecentSession() id=");
  OSDeviceDebug::putHex(m_sessionUniqueId);
  OSDeviceDebug::putString(", block=");
  OSDeviceDebug::putDec(m_sessionFirstBlockNumber);
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

  LargeCircularSessionsStorage::Header header;

  ret = getStorage().searchMostRecentlyWrittenBlock(&m_mostRecentlyWrittenBlockNumber,
      header);
  if (ret == OSReturn::OS_NOT_FOUND)
    {
      return ret;
    }

  m_sessionLastBlockNumber = m_mostRecentlyWrittenBlockNumber;

  for (;;)
    {
      SessionBlockNumber_t lastBlockNumber;
      lastBlockNumber = getStorage().computeCircularSessionBlockNumber(
          header.getSessionFirstBlockNumber(), -1);

      ret = getStorage().readStorageBlock(lastBlockNumber, m_ibuf, 1);
      if (ret != OSReturn::OS_OK)
        return ret;

      // We are now on the last block of the previous session

      // If not valid, quit loop, the previous header points to the
      // oldest session
      if (Header::readMagic(m_ibuf) != Header::MAGIC)
        break;

      SessionUniqueId_t lastBlockSessionUniqueId;
      lastBlockSessionUniqueId = Header::readSessionUniqueId(m_ibuf);

      SessionBlockNumber_t firstBlockNumber;
      firstBlockNumber = Header::readSessionFirstBlockNumber(m_ibuf);

      ret = getStorage().readStorageBlock(lastBlockNumber, m_ibuf, 1);
      if (ret != OSReturn::OS_OK)
        return ret;

      // We are now on the first block of the previous session
      if (Header::readMagic(m_ibuf) != Header::MAGIC)
        break;

      SessionUniqueId_t firstBlockSessionUniqueId;
      firstBlockSessionUniqueId = Header::readSessionUniqueId(m_ibuf);

      if (firstBlockSessionUniqueId != lastBlockSessionUniqueId)
        break;

      m_sessionLastBlockNumber = lastBlockNumber;
      header.readFromHeader(m_ibuf);
    }

  m_sessionUniqueId = header.getSessionUniqueId();
  m_sessionFirstBlockNumber = header.getSessionFirstBlockNumber();

  // TODO: should account for roll
  m_sessionLength = m_sessionLastBlockNumber - m_sessionFirstBlockNumber + 1;

  m_currentBlockNumber = m_sessionFirstBlockNumber;

  OSDeviceDebug::putString("openOldestSession() id=");
  OSDeviceDebug::putHex(m_sessionUniqueId);
  OSDeviceDebug::putString(", block=");
  OSDeviceDebug::putDec(m_sessionFirstBlockNumber);
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

  ret = getStorage().readStorageBlock(lastBlockNumber, m_ibuf, 1);
  if (ret != OSReturn::OS_OK)
    return ret;

  // We are now on the last block of the previous session

  // If not valid, quit loop, the previous header points to the
  // oldest session
  if (Header::readMagic(m_ibuf) != Header::MAGIC)
    return OSReturn::OS_END_OF_COLLECTION;

  SessionUniqueId_t lastBlockSessionUniqueId;
  lastBlockSessionUniqueId = Header::readSessionUniqueId(m_ibuf);

  SessionBlockNumber_t firstBlockNumber;
  firstBlockNumber = Header::readSessionFirstBlockNumber(m_ibuf);

  ret = getStorage().readStorageBlock(lastBlockNumber, m_ibuf, 1);
  if (ret != OSReturn::OS_OK)
    return ret;

  // We are now on the first block of the previous session
  if (Header::readMagic(m_ibuf) != Header::MAGIC)
    return OSReturn::OS_END_OF_COLLECTION;

  SessionUniqueId_t firstBlockSessionUniqueId;
  firstBlockSessionUniqueId = Header::readSessionUniqueId(m_ibuf);

  if (firstBlockSessionUniqueId != lastBlockSessionUniqueId)
    return OSReturn::OS_END_OF_COLLECTION;

  m_sessionUniqueId = firstBlockSessionUniqueId;
  m_sessionFirstBlockNumber = firstBlockNumber;

  m_sessionLastBlockNumber = lastBlockNumber;

  // TODO: should account for roll
  m_sessionLength = m_sessionLastBlockNumber - m_sessionFirstBlockNumber + 1;

  m_currentBlockNumber = m_sessionFirstBlockNumber;

  OSDeviceDebug::putString("openPreviousSession() id=");
  OSDeviceDebug::putHex(m_sessionUniqueId);
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
  m_currentBlockNumber = getStorage().computeCircularSessionBlockNumber(
      m_currentBlockNumber, 1);

  OSReturn_t ret;

  ret
      = getStorage().readStorageBlock(m_currentBlockNumber, m_ibuf, 1);

  if (ret == OSReturn::OS_OK)
    {
      Magic_t magic;
      magic = LargeCircularSessionsStorage::Header::readMagic(m_ibuf);

      // Check block magic
      if (magic == LargeCircularSessionsStorage::Header::MAGIC)
        {
          // If valid, read block session unique id
          m_sessionUniqueId
              = LargeCircularSessionsStorage::Header::readSessionUniqueId(
                  m_ibuf);

#if false
          OSDeviceDebug::putString("openNextSession() start block=");
          OSDeviceDebug::putDec(m_sessionFirstBlockNumber);
          OSDeviceDebug::putString(", crt block=");
          OSDeviceDebug::putDec(m_currentBlockNumber);
          OSDeviceDebug::putNewLine();
#endif
          ret = OSReturn::OS_OK;
        }
      else
        {
          OSDeviceDebug::putString("openNextSession() bad magic");
          OSDeviceDebug::putNewLine();

          ret = OSReturn::OS_BAD_MAGIC;
        }
    }


  return OSReturn::OS_NOT_IMPLEMENTED;
}

// Read the requested session block (minimum one device block)
// The block number should be between 0 and (sessionLength-1)
OSReturn_t
LargeCircularSessionsStorage::Reader::readSessionBlock(
    SessionBlockNumber_t blockNumber, uint8_t* pBlock,
    OSDeviceBlock::BlockCount_t deviceBlocksCount)
{
  if (deviceBlocksCount >= m_sessionLength)
    {
      return OSReturn::OS_OUT_OF_RANGE;
    }

  // Add blockNumber to session start
  m_currentBlockNumber = getStorage().computeCircularSessionBlockNumber(
      m_sessionFirstBlockNumber, (int) blockNumber);

#if true
  OSDeviceDebug::putString("readSessionBlock() blk=");
  OSDeviceDebug::putDec(blockNumber);
  OSDeviceDebug::putString(", storage blk=");
  OSDeviceDebug::putDec(m_currentBlockNumber);
  OSDeviceDebug::putNewLine();
#endif

  OSReturn_t ret;

  ret
      = getStorage().readStorageBlock(m_currentBlockNumber, pBlock, deviceBlocksCount);

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

          if (sessionUniqueId != m_sessionUniqueId)
            {
              // If the session id changes, return eof
              OSDeviceDebug::putString("readSessionBlock() eof");
              OSDeviceDebug::putNewLine();

              ret = OSReturn::OS_END_OF_COLLECTION;
            }

#if false
          OSDeviceDebug::putString("readSessionBlock() start block=");
          OSDeviceDebug::putDec(m_sessionFirstBlockNumber);
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

          ret = OSReturn::OS_BAD_MAGIC;
        }
    }

  return ret;
}

OSReturn_t
LargeCircularSessionsStorage::Reader::readNextSessionBlock(uint8_t* pBlock,
    OSDeviceBlock::BlockCount_t deviceBlocksCount)
{
  // Add blockNumber to session start
  m_currentBlockNumber = getStorage().computeCircularSessionBlockNumber(
      m_currentBlockNumber, 1);

  OSReturn_t ret;

  ret
      = getStorage().readStorageBlock(m_currentBlockNumber, pBlock, deviceBlocksCount);

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

          if (sessionUniqueId != m_sessionUniqueId)
            {
              // If the session id changes, return eof
              OSDeviceDebug::putString("readNextSessionBlock() eof");
              OSDeviceDebug::putNewLine();

              ret = OSReturn::OS_END_OF_COLLECTION;
            }

#if false
          OSDeviceDebug::putString("readNextSessionBlock() start block=");
          OSDeviceDebug::putDec(m_sessionFirstBlockNumber);
          OSDeviceDebug::putString(", crt block=");
          OSDeviceDebug::putDec(m_currentBlockNumber);
          OSDeviceDebug::putNewLine();
#endif
          ret = OSReturn::OS_OK;
        }
      else
        {
          OSDeviceDebug::putString("readNextSessionBlock() bad magic");
          OSDeviceDebug::putNewLine();

          ret = OSReturn::OS_BAD_MAGIC;
        }
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
