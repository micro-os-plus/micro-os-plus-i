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
  debug.putConstructor_P(PSTR("LargeCircularSessionsStorage"), this);

  m_blockSizeBlocks = 1;
}

LargeCircularSessionsStorage::~LargeCircularSessionsStorage()
{
  debug.putDestructor_P(PSTR("LargeCircularSessionsStorage"), this);
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
LargeCircularSessionsStorage::searchMostRecentWrittenBlock(
    SessionBlockNumber_t* plast, BlockUniqueId_t* pid)
{
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

  Header first;
  Header last;
  Header middle;

  SessionBlockNumber_t firstBlockNumber, lastBlockNumber, middleBlockNumber;

  bool ready; // true after mmc_init()
  ready = false;

  // search for last written block
  bool rollover;
  rollover = false;

  OSReturn_t r;

  unsigned char i;
  for (i = 0; i < 2; ++i)
    {
      firstBlockNumber = 0;
      lastBlockNumber = blk_max;
      //r = readBlkHdr(pBuf, blk_i, &id_i, &begblk_i);
      r = readStorageBlock(firstBlockNumber, pBuf, 1);
      if (r != OSReturn::OS_OK)
        {
          if (os.isDebug())
            {
              debug.putChar('V');
              debug.putHex((unsigned short) r);
            }
          return r;
        }
      //id_i = Header::getBlockUniqueId(pBuf);
      //begblk_i = Header::getSessionFirstBlockNumber(pBuf);
      first.copyFromHeader(pBuf);

      //r = readBlkHdr(pBuf, blk_j, &id_j, &begblk_j);
      r = readStorageBlock(lastBlockNumber, pBuf, 1);
      if (r != OSReturn::OS_OK)
        {
          if (os.isDebug())
            {
              debug.putChar('W');
              debug.putHex((unsigned short) r);
            }

          return r;
        }
      //id_j = Header::getBlockUniqueId(pBuf);
      // begblk_j = Header::getSessionFirstBlockNumber(pBuf);
      last.copyFromHeader(pBuf);

      // Is this the first pass?
      if (i == 0)
        {
          // Some additional checks required for the first pass
          if (first.getBlockUniqueId() == 0 && last.getBlockUniqueId() == 0
              && first.getSessionFirstBlockNumber() == 0
              && last.getSessionFirstBlockNumber() == 0)
            {
              // be sure we do not start at zero!
              blk_beg = blk_max; // start from last block
              id_beg = BEGINING_UNIQUE_ID;
              if (os.isDebug())
                {
                  debug.putChar('d');
                }
              ready = true;

              break;
            }

          if (((last.getBlockUniqueId() >> 24) == 0xFF)
              && ((first.getBlockUniqueId() >> 24) == 0x00))
            {
              // ID rollover right at last block
              rollover = true;

              continue;
            }

          if (first.getBlockUniqueId() < last.getBlockUniqueId())
            {
              blk_beg = 0;
              id_beg = last.getBlockUniqueId() + 1;
              if (os.isDebug())
                {
                  debug.putChar('i');
                }
              ready = true;

              break;
            }
        }
      else
        {
          // Second pass
          if ((signed long) first.getBlockUniqueId()
              < (signed long) last.getBlockUniqueId())
            {
              blk_beg = 0;
              id_beg = last.getBlockUniqueId() + 1;
              if (os.isDebug())
                {
                  debug.putChar('I');
                }
              ready = true;

              break;
            }
        }

      for (; lastBlockNumber - firstBlockNumber > 1;)
        {
          if (i == 0)
            {
              if (((first.getBlockUniqueId() >> 24) == 0xFF)
                  && ((last.getBlockUniqueId() >> 24) == 0x00))
                {
                  // Rollover occured
                  rollover = true;
                  break;
                }
            }

          middleBlockNumber = (firstBlockNumber + lastBlockNumber) / 2;
          //r = readBlkHdr(pBuf, blk_k, &id_k, &begblk_k);
          r = readStorageBlock(middleBlockNumber, pBuf, 1);
          if (r != OSReturn::OS_OK)
            {
              if (os.isDebug())
                {
                  debug.putChar('X');
                  debug.putHex((unsigned short) r);
                }

              return r;
            }
          //id_k = Header::getBlockUniqueId(pBuf);
          //begblk_k = Header::getSessionFirstBlockNumber(pBuf);
          middle.copyFromHeader(pBuf);

#if 1
          if (os.isDebug())
            {
              debug.putNewLine();
              debug.putHex(firstBlockNumber);
              debug.putChar('-');
              debug.putHex(first.getBlockUniqueId());
              debug.putChar(' ');
              debug.putHex(middleBlockNumber);
              debug.putChar('-');
              debug.putHex(middle.getBlockUniqueId());
              debug.putChar(' ');
              debug.putHex(lastBlockNumber);
              debug.putChar('-');
              debug.putHex(last.getBlockUniqueId());
            }
#endif

          if (i == 0)
            {
              if ((middle.getBlockUniqueId() < first.getBlockUniqueId())
                  || (middle.getBlockUniqueId() == 0))
                {
                  lastBlockNumber = middleBlockNumber;
                  last.setBlockUniqueId(middle.getBlockUniqueId());
                  last.setSessionFirstBlockNumber(
                      middle.getSessionFirstBlockNumber());
                }
              else
                {
                  firstBlockNumber = middleBlockNumber;
                  first.setBlockUniqueId(middle.getBlockUniqueId());
                  first.setSessionFirstBlockNumber(
                      middle.getSessionFirstBlockNumber());
                }
            }
          else
            {
              // Roll over case: use signed comparisons
              if (((signed long) middle.getBlockUniqueId()
                  < (signed long) first.getBlockUniqueId())
                  || (middle.getBlockUniqueId() == 0))
                {
                  lastBlockNumber = middleBlockNumber;
                  last.setBlockUniqueId(middle.getBlockUniqueId());
                  last.setSessionFirstBlockNumber(
                      middle.getSessionFirstBlockNumber());
                }
              else
                {
                  firstBlockNumber = middleBlockNumber;
                  first.setBlockUniqueId(middle.getBlockUniqueId());
                  first.setSessionFirstBlockNumber(
                      middle.getSessionFirstBlockNumber());
                }
            }
        }

      blk_beg = lastBlockNumber;
      id_beg = first.getBlockUniqueId() + 1;

      if (!rollover || i == 1)
        {
#if 0
          // Consistency test to prevent overwriting useful data
          // Warning: single session memory roll over case???


          if ( first.getSessionFirstBlockNumber() != last.getSessionFirstBlockNumber() )
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
}

int
LargeCircularSessionsStorage::readBlkHdr(uint8_t *pBlock,
    SessionBlockNumber_t sessionBlockNumber, BlockUniqueId_t* pBlockUniqueId,
    SessionBlockNumber_t* pSessionBlockNumber, uint_t rtry)
{
  OSReturn_t res;
  res = 0;

  unsigned int j;
  for (j = rtry + 1; j != 0; --j)
    {
      // Read the first device block of the session block
      res = readStorageBlock(sessionBlockNumber, pBlock, 1);
      if (res == OSReturn::OS_OK)
        {
          if (pBlockUniqueId != 0)
            {
              *pBlockUniqueId
                  = LargeCircularSessionsStorage::Header::getBlockUniqueId(
                      pBlock);
            }

          if (pSessionBlockNumber != 0)
            {
              *pSessionBlockNumber
                  = LargeCircularSessionsStorage::Header::getSessionFirstBlockNumber(
                      pBlock);
            }

          break;

        }
      //os.busyWaitMillis(1);
    }

  return res;
}

// ============================================================================

// ----- Constructors & destructors -------------------------------------------

LargeCircularSessionsStorage::Header::Header()
{
  debug.putConstructor_P(PSTR("LargeCircularSessionsStorage::Header"), this);
}

LargeCircularSessionsStorage::Header::~Header()
{
  debug.putDestructor_P(PSTR("LargeCircularSessionsStorage::Header"), this);
}

// ----- Public methods -------------------------------------------------------

void
LargeCircularSessionsStorage::Header::copyFromHeader(uint8_t* pHeader)
{
  setMagic(getMagic(pHeader));
  setSessionUniqueId(getSessionUniqueId(pHeader));
  setSessionFirstBlockNumber(getSessionFirstBlockNumber(pHeader));
  setBlockUniqueId(getBlockUniqueId(pHeader));
}

// ----- Static public methods ------------------------------------------------

void
LargeCircularSessionsStorage::Header::setMagic(uint8_t* pHeader, Magic_t value)
{
  uint8_t* p;
  p = pHeader + OFFSET_OF_MAGIC + sizeof(value);

  for (uint_t i = 0; i < sizeof(value); ++i)
    {
      *--p = (value & 0xFF);
      value >>= 8;
    }
}

LargeCircularSessionsStorage::Magic_t
LargeCircularSessionsStorage::Header::getMagic(uint8_t* pHeader)
{
  Magic_t value;
  value = 0;

  uint8_t* p;
  p = pHeader + OFFSET_OF_MAGIC + sizeof(value);

  for (uint_t i = 0; i < sizeof(value); ++i)
    {
      value |= *--p;
      value <<= 8;
    }

  return value;
}

void
LargeCircularSessionsStorage::Header::setSessionUniqueId(uint8_t* pHeader,
    SessionUniqueId_t value)
{
  uint8_t* p;
  p = pHeader + OFFSET_OF_SESSIONUNIQEID + sizeof(value);

  for (uint_t i = 0; i < sizeof(value); ++i)
    {
      *--p = (value & 0xFF);
      value >>= 8;
    }
}

LargeCircularSessionsStorage::SessionUniqueId_t
LargeCircularSessionsStorage::Header::getSessionUniqueId(uint8_t* pHeader)
{
  SessionUniqueId_t value;
  value = 0;

  uint8_t* p;
  p = pHeader + OFFSET_OF_SESSIONUNIQEID + sizeof(value);

  for (uint_t i = 0; i < sizeof(value); ++i)
    {
      value |= *--p;
      value <<= 8;
    }

  return value;
}

void
LargeCircularSessionsStorage::Header::setSessionFirstBlockNumber(
    uint8_t* pHeader, SessionBlockNumber_t value)
{
  uint8_t* p;
  p = pHeader + OFFSET_OF_SESSIONFIRSTBLOCKNUMBER + sizeof(value);

  for (uint_t i = 0; i < sizeof(value); ++i)
    {
      *--p = (value & 0xFF);
      value >>= 8;
    }
}

LargeCircularSessionsStorage::SessionBlockNumber_t
LargeCircularSessionsStorage::Header::getSessionFirstBlockNumber(
    uint8_t* pHeader)
{
  SessionBlockNumber_t value;
  value = 0;

  uint8_t* p;
  p = pHeader + OFFSET_OF_SESSIONFIRSTBLOCKNUMBER + sizeof(value);

  for (uint_t i = 0; i < sizeof(value); ++i)
    {
      value |= *--p;
      value <<= 8;
    }

  return value;
}

void
LargeCircularSessionsStorage::Header::setBlockUniqueId(uint8_t* pHeader,
    BlockUniqueId_t value)
{
  uint8_t* p;
  p = pHeader + OFFSET_OF_BLOCKUNIQUEID + sizeof(value);

  for (uint_t i = 0; i < sizeof(value); ++i)
    {
      *--p = (value & 0xFF);
      value >>= 8;
    }
}

LargeCircularSessionsStorage::BlockUniqueId_t
LargeCircularSessionsStorage::Header::getBlockUniqueId(uint8_t* pHeader)
{
  BlockUniqueId_t value;
  value = 0;

  uint8_t* p;
  p = pHeader + OFFSET_OF_BLOCKUNIQUEID + sizeof(value);

  for (uint_t i = 0; i < sizeof(value); ++i)
    {
      value |= *--p;
      value <<= 8;
    }

  return value;
}

// ============================================================================

// ----- Constructors & destructors -------------------------------------------

LargeCircularSessionsStorage::Writer::Writer(
    LargeCircularSessionsStorage& storage) :
  m_storage(storage)
{
  debug.putConstructor_P(PSTR("LargeCircularSessionsStorage::Writer"), this);
}

LargeCircularSessionsStorage::Writer::~Writer()
{
  debug.putDestructor_P(PSTR("LargeCircularSessionsStorage::Writer"), this);
}

// ----- Public methods -------------------------------------------------------

OSReturn_t
LargeCircularSessionsStorage::Writer::createSession(
    SessionUniqueId_t sessionUniqueId)
{
  if (sessionUniqueId == 0)
    {
      return OSReturn::OS_BAD_PARAMETER;
    }

#if true
  m_sessionUniqueId = sessionUniqueId;

  // TODO: search for the next free block;
  // For the moment just start from the beginning
  // and with a dummy block unique id

  m_sessionFirstBlockNumber = 0;
  m_currentBlockNumber = 0;

  m_blockUniqueId = 0x12345678;
#else
  getStorage().searchMostRecentWrittenBlock();
#endif

  return getStorage().getDevice().open();
}

OSReturn_t
LargeCircularSessionsStorage::Writer::closeSession(void)
{
  m_sessionUniqueId = 0;

  return getStorage().getDevice().close();
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

  LargeCircularSessionsStorage::Header::setMagic(pBlock,
      LargeCircularSessionsStorage::Header::MAGIC);
  LargeCircularSessionsStorage::Header::setSessionUniqueId(pBlock,
      m_sessionUniqueId);
  LargeCircularSessionsStorage::Header::setSessionFirstBlockNumber(pBlock,
      m_sessionFirstBlockNumber);
  LargeCircularSessionsStorage::Header::setBlockUniqueId(pBlock,
      m_blockUniqueId);

  OSReturn_t ret;

  // Write a full session block to the output device
  ret = getStorage().writeStorageBlock(m_currentBlockNumber, pBlock, nBlocks);

  // Increment the block number; when reaching storage size, roll over
  m_currentBlockNumber++;
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
  debug.putConstructor_P(PSTR("LargeCircularSessionsStorage::Reader"), this);
}

LargeCircularSessionsStorage::Reader::~Reader()
{
  debug.putDestructor_P(PSTR("LargeCircularSessionsStorage::Reader"), this);
}

// ----- Public methods -------------------------------------------------------

// Searches for the given session
OSReturn_t
LargeCircularSessionsStorage::Reader::openSession(SessionUniqueId_t sessionId)
{
  // TODO: implement

  m_sessionFirstBlockNumber = 0;

  return OSReturn::OS_OK;
}

// Without a session id, search for the most recent one
OSReturn_t
LargeCircularSessionsStorage::Reader::openMostRecentSession(void)
{
  // TODO: implement

  m_sessionFirstBlockNumber = 0;

  return OSReturn::OS_OK;
}

OSReturn_t
LargeCircularSessionsStorage::Reader::openOldestSession(void)
{
  // TODO: search
  m_sessionFirstBlockNumber = 0;

  OSReturn_t ret;
  ret = getStorage().readStorageBlock(m_sessionFirstBlockNumber, m_ibuf, 1);

  if (ret == OSReturn::OS_OK)
    {
      Magic_t magic;
      magic = LargeCircularSessionsStorage::Header::getMagic(m_ibuf);
      if (magic == LargeCircularSessionsStorage::Header::MAGIC)
        {
          // remember session unique id
          m_sessionUniqueId
              = LargeCircularSessionsStorage::Header::getSessionUniqueId(m_ibuf);

          debug.putString("oldest session id=");
          debug.putHex(m_sessionUniqueId);
          debug.putString(", block=");
          debug.putDec(m_sessionFirstBlockNumber);
          debug.putNewLine();

          ret = OSReturn::OS_OK;
        }
      else
        {
          debug.putString("open oldest bad magic");
          debug.putNewLine();

          ret = OSReturn::OS_BAD_MAGIC;
        }
    }
  return ret;
}

OSReturn_t
LargeCircularSessionsStorage::Reader::openPreviousSession(void)
{
  // TODO: implement
  return OSReturn::OS_NOT_IMPLEMENTED;
}

OSReturn_t
LargeCircularSessionsStorage::Reader::openNextSession(void)
{
  // TODO: implement
  return OSReturn::OS_NOT_IMPLEMENTED;
}

// Compute the length of the current session, in session blocks
LargeCircularSessionsStorage::SessionBlockCount_t
LargeCircularSessionsStorage::Reader::computeSessionLength(void)
{
  // TODO: implement
  return 0;
}

// Read the requested session block (minimum one device block)
// The block number should be between 0 and (sessionLength-1)
OSReturn_t
LargeCircularSessionsStorage::Reader::readSessionBlock(
    SessionBlockNumber_t blockNumber, uint8_t* pBuf,
    OSDeviceBlock::BlockCount_t deviceBlocksCount)
{
  SessionBlockNumber_t newBlockNumber;
  // Add blockNumber to session start
  newBlockNumber = getStorage().computeCircularSessionBlockNumber(
      m_sessionFirstBlockNumber, (int) blockNumber);

  OSReturn_t ret;

  ret = getStorage().readStorageBlock(newBlockNumber, pBuf, deviceBlocksCount);

  if (ret == OSReturn::OS_OK)
    {
      Magic_t magic;
      magic = LargeCircularSessionsStorage::Header::getMagic(m_ibuf);

      // Check block magic
      if (magic == LargeCircularSessionsStorage::Header::MAGIC)
        {
          // If valid, read block session unique id
          SessionUniqueId_t sessionUniqueId;
          sessionUniqueId
              = LargeCircularSessionsStorage::Header::getSessionUniqueId(m_ibuf);

          if (sessionUniqueId != m_sessionUniqueId)
            {
              // If the session id changes, return eof
              debug.putString("read session block eof");
              debug.putNewLine();

              ret = OSReturn::OS_END_OF_COLLECTION;
            }

          debug.putString("read session block=");
          debug.putDec(m_sessionFirstBlockNumber);
          debug.putNewLine();

          ret = OSReturn::OS_OK;
        }
      else
        {
          debug.putString("read session block bad magic");
          debug.putNewLine();

          ret = OSReturn::OS_BAD_MAGIC;
        }
    }

  return ret;
}

OSReturn_t
LargeCircularSessionsStorage::Reader::closeSession(void)
{
  // TODO: implement
  return OSReturn::OS_OK;
}

// ----------------------------------------------------------------------------

// ============================================================================

#endif /* defined(OS_INCLUDE_LARGECIRCULARSESSIONSSTORAGE) */
