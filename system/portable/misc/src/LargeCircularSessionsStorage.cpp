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
    OSDeviceBlock& device, OSEventNotifier* pEventNotifier) :
  m_device(device)
{
  OSDeviceDebug::putConstructor_P(PSTR("LargeCircularSessionsStorage"), this);

  m_blockSizeBlocks = 1;

  m_event = (OSEvent_t) (&m_event);
  m_pEventNotifier = pEventNotifier;
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
#if defined(OS_DEBUG_LARGECIRCULARSESSIONSTORAGE_READSTORAGEBLOCK)
  OSDeviceDebug::putString(" readStorageBlock() blk=");
  OSDeviceDebug::putDec(blockNumber);
  OSDeviceDebug::putNewLine();
#endif /* OS_DEBUG_LARGECIRCULARSESSIONSTORAGE_READSTORAGEBLOCK */

  // Validate block count
  if (deviceBlocksCount > getSessionBlockSizeBlocks())
    {
      OSDeviceDebug::putString(" readStorageBlock() OUT_OF_RANGE ");
      return OSReturn::OS_OUT_OF_RANGE;
    }

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
    {
      OSDeviceDebug::putString(" writeStorageBlock() OUT_OF_RANGE ");
      return OSReturn::OS_OUT_OF_RANGE;
    }

  // Compute the device address and call the device write
  return getDevice().writeBlocks(blockNumber * getSessionBlockSizeBlocks(),
      pSessionBlock, deviceBlocksCount);
}

OSReturn_t
LargeCircularSessionsStorage::searchMostRecentlyWrittenBlock(
    uint8_t* pBlockBuffer, LargeCircularSessionsStorage::SessionBlock& block,
    LargeCircularSessionsStorage::Session& session)
{
  OSDeviceDebug::putString(" searchMostRecentlyWrittenBlock() ");

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

  ret = readStorageBlock(leftBlockNumber, pBlockBuffer, 1);
  if (ret != OSReturn::OS_OK)
    {
      return ret;
    }
  left.readFromBlock(pBlockBuffer);

  if (!left.isMagicValid())
    {
      // If the first block does not have a magic, then we consider
      // the entire partition to be empty.
      OSDeviceDebug::putString(" NOT_FOUND ");
      return OSReturn::OS_NOT_FOUND;
    }

  ret = readStorageBlock(rightBlockNumber, pBlockBuffer, 1);
  if (ret != OSReturn::OS_OK)
    {
      return ret;
    }
  right.readFromBlock(pBlockBuffer);

  if (!right.isMagicValid())
    {
      // If the last block does not have a magic, then we consider
      // the entire partition to be linear, without roll, so we just
      // do a binary search of the last block with valid magic

      OSDeviceDebug::putString(" search ");
      for (;;)
        {
          OSDeviceDebug::putChar(' ');
          OSDeviceDebug::putDec(leftBlockNumber);
          OSDeviceDebug::putChar('-');
          OSDeviceDebug::putDec(rightBlockNumber);
          OSDeviceDebug::putChar(' ');

          if (leftBlockNumber + 1 == rightBlockNumber)
            {
              // Found, the 'left' header is the most recent
              break;
            }

          middleBlockNumber = (leftBlockNumber + rightBlockNumber) / 2;
          ret = readStorageBlock(middleBlockNumber, pBlockBuffer, 1);
          if (ret != OSReturn::OS_OK)
            {
              return ret;
            }
          middle.readFromBlock(pBlockBuffer);

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

      // To make the previous result more visible
      OSDeviceDebug::putNewLine();

      // Found, the left block is the most recently written
      // The right is the first empty block, with invalid magic

      block.setBlockNumber(leftBlockNumber);
      block.setUniqueId(left.getBlockUniqueId());

      session.setUniqueId(left.getSessionUniqueId());
      session.setFirstBlockNumber(left.getSessionFirstBlockNumber());

      goto checkReferenceToNext;
    }

  // Check if the discontinuity is exactly at the circular border
  if (right.getBlockUniqueId() + 1 != left.getBlockUniqueId())
    {
      // Found, the right block (the last in the array) is the most recently
      // written. The left block (the first in the array) is the oldest
      // block, with the oldest block unique id, that we'll overwrite

      block.setBlockNumber(rightBlockNumber);
      block.setUniqueId(right.getBlockUniqueId());

      session.setUniqueId(right.getSessionUniqueId());
      session.setFirstBlockNumber(right.getSessionFirstBlockNumber());

      goto checkReferenceToNext;
    }

  // After checking the border, the circular storage is now simplified to a
  // linear storage, so a binary search, similar to the above, would do
  // the job

  OSDeviceDebug::putString(" search ");
  for (;;)
    {
      OSDeviceDebug::putChar(' ');
      OSDeviceDebug::putDec(leftBlockNumber);
      OSDeviceDebug::putChar('-');
      OSDeviceDebug::putDec(rightBlockNumber);
      OSDeviceDebug::putChar(' ');

      if (leftBlockNumber + 1 == rightBlockNumber)
        {
          // Found, the 'left' header is the most recent
          break;
        }

      middleBlockNumber = (leftBlockNumber + rightBlockNumber) / 2;
      ret = readStorageBlock(middleBlockNumber, pBlockBuffer, 1);
      if (ret != OSReturn::OS_OK)
        {
          return ret;
        }
      middle.readFromBlock(pBlockBuffer);

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

  // To make the previous result more visible
  OSDeviceDebug::putNewLine();

  // Found, the left block is the most recently written.
  // The right is the oldest written block, that we'll overwrite

  block.setBlockNumber(leftBlockNumber);
  block.setUniqueId(left.getBlockUniqueId());

  // Store the session unique ID
  session.setUniqueId(left.getSessionUniqueId());
  //  and the first block number
  session.setFirstBlockNumber(left.getSessionFirstBlockNumber());

  checkReferenceToNext: // label

  // Store the last session block number
  session.setLastBlockNumber(block.getBlockNumber());

  // Read the first block of the session, to get nextSession pointer
  ret = readStorageBlock(session.getFirstBlockNumber(), pBlockBuffer, 1);
  if (ret == OSReturn::OS_OK)
    {
      // Try to get pointer to next session
      session.setNextSessionFirstBlockNumber(
          Header::readNextSessionFirstBlockNumber(pBlockBuffer));

      session.setLength(
          computeCircularSessionLength(session.getFirstBlockNumber(),
              session.getLastBlockNumber()));

      // If the session was properly closed
      if (session.getNextSessionFirstBlockNumber()
          != Header::INVALID_BLOCKNUMBER)
        {
          session.setCompletelyWritten(true);
        }
    }
  return ret;

#endif
}

OSReturn_t
LargeCircularSessionsStorage::updateForwardReference(
    SessionBlockNumber_t sessionFirstBlockNumber,
    SessionBlockNumber_t nextSessionFirstBlockNumber, uint8_t* pBlock)
{
  OSDeviceDebug::putString(" updateForwardReference()");
  OSDeviceDebug::putNewLine();

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

void
LargeCircularSessionsStorage::clearCachedValues(void)
{
  m_mostRecentlyWrittenSession.setFirstBlockNumber(Header::INVALID_BLOCKNUMBER);
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
  m_nextSessionFirstBlockNumber = Header::INVALID_BLOCKNUMBER;

  //m_currentBlockNumber = 0;
}

LargeCircularSessionsStorage::Header::~Header()
{
  OSDeviceDebug::putDestructor_P(PSTR("LargeCircularSessionsStorage::Header"),
      this);
}

// ----- Public methods -------------------------------------------------------

#if false
LargeCircularSessionsStorage::Header&
LargeCircularSessionsStorage::Header::operator=(const Header& header)
  {
    Header& h = ((Header&) header);

    // Copy all members from the parameter instance
    setMagic(h.getMagic());
    setBlockUniqueId(h.getBlockUniqueId());
    setFirstBlockNumber(h.getFirstBlockNumber());
    setUniqueId(h.getUniqueId());
    setNextSessionFirstBlockNumber(h.getNextSessionFirstBlockNumber());

    return *this;
  }
#endif

#if true
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
#endif

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
  m_sessionFirstBlockNumber = Header::INVALID_BLOCKNUMBER;
  m_sessionFirstBlockNumber = Header::INVALID_BLOCKNUMBER;
  m_sessionLength = 0;
  m_isCompletelyWritten = false;
  m_nextSessionFirstBlockNumber = Header::INVALID_BLOCKNUMBER;
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
  OSDeviceDebug::putString(" createSession() sid=");
  OSDeviceDebug::putHex(m_currentSession.getUniqueId());
  OSDeviceDebug::putChar(' ');

  OSReturn_t ret;
  ret = OSReturn::OS_OK;

  LargeCircularSessionsStorage::Session mostRecentSession;
  LargeCircularSessionsStorage::SessionBlock mostRecentBlock;

  if (!getStorage().getMostRecentlyWrittenSession().isValid())
    {
      // This is the first call, we need to search the storage for the
      // most recently written block

      ret = getStorage().searchMostRecentlyWrittenBlock(m_blockBuffer,
          mostRecentBlock, mostRecentSession);

      if (ret == OSReturn::OS_NOT_FOUND)
        {
          // No sessions in the storage

          // Initialise the storage to start from the beginning
          m_currentBlock.setBlockNumber(0);
          m_currentBlock.setUniqueId(1);

          // Initialise the session
          if (sessionUniqueId != NEXT_SESSSION_UNIQUE_ID)
            {
              // Use the requested session unique id
              m_currentSession.setUniqueId(sessionUniqueId);
            }
          else
            {
              // If null, start with 1
              m_currentSession.setUniqueId(1);
            }

          m_currentSession.setFirstBlockNumber(m_currentBlock.getBlockNumber());

          // last, next and length not initialised

          ret = OSReturn::OS_OK;
        }
      else if (ret == OSReturn::OS_OK)
        {
          // At least one session found, use it as the most recent block

          // Compute the next block
          m_currentBlock.setBlockNumber(
              getStorage().computeCircularSessionBlockNumber(
                  mostRecentBlock.getBlockNumber(), 1));

          // The new block id is just next to the previous one
          m_currentBlock.setUniqueId(mostRecentBlock.getUniqueId() + 1);

          // Check if forward reference is updated
          if (!mostRecentSession.isForwardReferenceValid())
            {
              // If not, update it, to keep storage consistent
              getStorage().updateForwardReference(
                  mostRecentSession.getFirstBlockNumber(),
                  m_currentBlock.getBlockNumber(), m_blockBuffer);
            }

          // Initialise the new session
          if (sessionUniqueId != NEXT_SESSSION_UNIQUE_ID)
            {
              if (sessionUniqueId <= mostRecentSession.getUniqueId())
                {
                  OSDeviceDebug::putString(" BAD_PARAMETER_(sid not monotone) ");
                  return OSReturn::OS_BAD_PARAMETER;
                }
              // Use the requested session unique id
              m_currentSession.setUniqueId(sessionUniqueId);
            }
          else
            {
              // If null, just increment the previous unique id
              m_currentSession.setUniqueId(mostRecentSession.getUniqueId() + 1);
            }

          m_currentSession.setFirstBlockNumber(m_currentBlock.getBlockNumber());

          // last, next and length not initialised

          ret = OSReturn::OS_OK;
        }
      else
        {
          return ret;
        }
    }
  else
    {
      // Subsequent sessions, i.e. most recently written block is known

      // The forward reference should have been updated at closeSession().

      // The m_currentBlockNumber and the m_currentHeader header were already
      // updated by the previous writeSessionBlock()

      if (sessionUniqueId != NEXT_SESSSION_UNIQUE_ID)
        {
          if (sessionUniqueId <= m_currentSession.getUniqueId())
            {
              OSDeviceDebug::putString(" BAD_PARAMETER_(sid not monotone) ");
              return OSReturn::OS_BAD_PARAMETER;
            }
          // Use the requested session unique id
          m_currentSession.setUniqueId(sessionUniqueId);
        }
      else
        {
          // If null, just increment the previous unique id
          m_currentSession.incrementUniqueId();
        }

      m_currentSession.setFirstBlockNumber(m_currentBlock.getBlockNumber());
    }

  // Initialise defaults for new sessions
  m_currentSession.setNextSessionFirstBlockNumber(Header::INVALID_BLOCKNUMBER);
  m_currentSession.setLastBlockNumber(Header::INVALID_BLOCKNUMBER);
  m_currentSession.setLength(0);
  m_currentSession.setCompletelyWritten(false);

  OSDeviceDebug::putString(" createSession() done bid=");
  OSDeviceDebug::putHex(m_currentBlock.getUniqueId());
  OSDeviceDebug::putString(", bno=");
  OSDeviceDebug::putDec(m_currentSession.getFirstBlockNumber());
  OSDeviceDebug::putNewLine();

  os.sched.lock.enter();
    {
      // Update storage to point to this newly created session
      // However, no new blocks are written, readers must wait.

      getStorage().getMostRecentlyWrittenSession() = m_currentSession;
      getStorage().getMostRecentlyWrittenSessionBlock() = m_currentBlock;
    }
  os.sched.lock.exit();

  // Inform readers that we are now in a writing session
  getStorage().setIsWriting(true);

  // Inform readers that a new session was created
  notifyReaders();

  return ret;
}

OSReturn_t
LargeCircularSessionsStorage::Writer::writeSessionBlock(uint8_t* pBlockBuffer)
{
  OSDeviceDebug::putString(" wsb=");
  OSDeviceDebug::putDec(m_currentBlock.getBlockNumber());
  OSDeviceDebug::putChar(' ');

  if (!m_currentSession.isValid())
    {
      OSDeviceDebug::putString(" NOT_INITIALISED ");
      return OSReturn::OS_NOT_INITIALISED;
    }

  SessionBlockNumber_t maxBlockNumber;
  maxBlockNumber = getStorage().computeCircularSessionBlockNumber(
      m_currentSession.getFirstBlockNumber(), -1);

  if (m_currentBlock.getBlockNumber() == maxBlockNumber)
    {
      OSDeviceDebug::putString(" SIZE_EXCEEDED ");
      return OSReturn::OS_SIZE_EXCEEDED;
    }

  // Prepare the current block header.
  Header::writeMagic(Header::MAGIC, pBlockBuffer);

  Header::writeSessionUniqueId(m_currentSession.getUniqueId(), pBlockBuffer);

  Header::writeSessionFirstBlockNumber(m_currentSession.getFirstBlockNumber(),
      pBlockBuffer);

  // All new blocks have the forward reference set to an illegal value.
  Header::writeNextSessionFirstBlockNumber(Header::INVALID_BLOCKNUMBER,
      pBlockBuffer);

  // Block unique id
  Header::writeBlockUniqueId(m_currentBlock.getUniqueId(), pBlockBuffer);

  // Write a full session block to the output device
  OSReturn_t ret;
  ret = getStorage().writeStorageBlock(m_currentBlock.getBlockNumber(),
      pBlockBuffer, getStorage().getSessionBlockSizeBlocks());

  // Update session to remember the written block as the last session block
  m_currentSession.setLastBlockNumber(m_currentBlock.getBlockNumber());
  m_currentSession.setLength(
      getStorage().computeCircularSessionLength(
          m_currentSession.getLastBlockNumber(),
          m_currentSession.getFirstBlockNumber()));

  os.sched.lock.enter();
    {
      Session& session = getStorage().getMostRecentlyWrittenSession();
      // Update common storage session class to point to the written block
      session.setLastBlockNumber(m_currentSession.getLastBlockNumber());
      session.setLength(m_currentSession.getLength());

      // Update common storage session block class to point to the written block
      getStorage().getMostRecentlyWrittenSessionBlock() = m_currentBlock;
    }
  os.sched.lock.exit();

  // Prepare for the next write or the next session

  // Increment the block number
  m_currentBlock.setBlockNumber(
      getStorage().computeCircularSessionBlockNumber(
          m_currentBlock.getBlockNumber(), 1));

  // Increment the monotone increasing unique block number
  m_currentBlock.incrementUniqueId();

  // Inform readers that a new block was added
  notifyReaders();

  return ret;
}

OSReturn_t
LargeCircularSessionsStorage::Writer::closeSession(void)
{
  OSDeviceDebug::putString(" closeSession() sid=");
  OSDeviceDebug::putHex(m_currentSession.getUniqueId());
  OSDeviceDebug::putString(", nbid=");
  OSDeviceDebug::putHex(m_currentBlock.getUniqueId());
  OSDeviceDebug::putString(", nbno=");
  OSDeviceDebug::putDec(m_currentBlock.getBlockNumber());
  OSDeviceDebug::putNewLine();

  if (!m_currentSession.isValid())
    {
      OSDeviceDebug::putString(" NOT_INITIALISED ");
      return OSReturn::OS_NOT_INITIALISED;
    }

  // Update the header of the current session to point to the next (future)
  // session. This is to help forward navigation, since backward
  // navigation is already possible, all blocks point to the first
  // session block
  OSReturn_t ret;
  ret = getStorage().updateForwardReference(
      m_currentSession.getFirstBlockNumber(), m_currentBlock.getBlockNumber(),
      m_blockBuffer);

  // Update pointer to next block to allow forward references
  m_currentSession.setNextSessionFirstBlockNumber(
      m_currentBlock.getBlockNumber());

  // Update session length to allow the reader to complete
  m_currentSession.setLength(
      getStorage().computeCircularSessionLength(
          m_currentSession.getLastBlockNumber(),
          m_currentSession.getFirstBlockNumber()));
  m_currentSession.setCompletelyWritten(true);

  os.sched.lock.enter();
    {
      LargeCircularSessionsStorage::Session storageSession =
          getStorage().getMostRecentlyWrittenSession();

      // Update common storage session class with session length
      storageSession.setLength(m_currentSession.getLength());

      // Update common storage session class to point to the next free block
      storageSession.setNextSessionFirstBlockNumber(
          m_currentBlock.getBlockNumber());
    }
  os.sched.lock.exit();

  // Inform readers that we are no longer in a writing session
  getStorage().setIsWriting(false);

  // Inform readers that the session was closed
  notifyReaders();

  return ret;
}

void
LargeCircularSessionsStorage::Writer::notifyReaders(void)
{
  OSEventNotifier* pEventNotifier;
  pEventNotifier = getStorage().getEventNotifier();
  if (pEventNotifier != NULL)
    {
      OSEvent_t event;
      event = getStorage().getEvent();

      pEventNotifier->eventNotify(event, (OSEventWaitReturn_t) event);
    }
}

// ============================================================================

// ----- Constructors & destructors -------------------------------------------

LargeCircularSessionsStorage::Reader::Reader(
    LargeCircularSessionsStorage& storage) :
  m_storage(storage), m_readSessionBlockCondition(*this)
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
  OSDeviceDebug::putString(" openSession() sid=");
  OSDeviceDebug::putHex(m_currentSession.getUniqueId());
  OSDeviceDebug::putChar(' ');

  OSReturn_t ret;
  ret = OSReturn::OS_OK;

  while (!getStorage().getMostRecentlyWrittenSession().isValid())
    {
      // If there was no prior write, we need to search.
      // The result is in mostRecentlyWrittenBlock
      ret = getStorage().searchMostRecentlyWrittenBlock(m_blockBuffer,
          m_currentBlock, m_currentSession);

      if (ret == OSReturn::OS_NOT_FOUND)
        {
          // No sessions in the storage

          if (doNotBlock)
            {
              OSDeviceDebug::putString(" WOULD_BLOCK ");
              return OSReturn::OS_WOULD_BLOCK;
            }

          // Wait for the writer to create a new session
          OSEvent_t event;
          event = getStorage().getEvent();

          // TODO: use scheduler lock to avoid race condition
          OSEventWaitReturn_t eventWaitReturn;
          eventWaitReturn = os.sched.eventWait(event);
          if (eventWaitReturn == OSEventWaitReturn::OS_CANCELLED)
            {
              return OSReturn::OS_CANCELLED;
            }
          continue;
        }
      else if (ret != OSReturn::OS_OK)
        {
          return ret;
        }

      os.sched.lock.enter();
        {
          if (!getStorage().getMostRecentlyWrittenSession().isValid())
            {
              // Update storage cache to point to this last session

              getStorage().getMostRecentlyWrittenSession() = m_currentSession;
              getStorage().getMostRecentlyWrittenSessionBlock()
                  = m_currentBlock;
            }
        }
      os.sched.lock.exit();
    }

  os.sched.lock.enter();
    {
      // Get the most recently written session from the storage cache
      m_currentSession = getStorage().getMostRecentlyWrittenSession();
    }
  os.sched.lock.exit();

  // Now m_currentSession points to the most recently written session
  SessionUniqueId_t mostRecentSessionUniqueId;
  mostRecentSessionUniqueId = m_currentSession.getUniqueId();

  // Did we ask for the newest session or the session we
  // asked is exactly the most recently written session?
  if ((sessionId == NEWEST_SESSSION_UNIQUE_ID) || (sessionId
      == mostRecentSessionUniqueId))
    {
      goto sessionFound;
    }

  // Did we ask for a future session?
  if (sessionId > mostRecentSessionUniqueId)
    {
#if true
      OSDeviceDebug::putString(" END_OF_COLLECTION ");
      return OSReturn::OS_END_OF_COLLECTION;
#else
      // Then we have to consider waiting for it
      if (doNotBlock)
        {
          OSDeviceDebug::putString(
              "openSession() would block on future session");
          return OSReturn::OS_WOULD_BLOCK;
        }

      for (;;)
        {
          // Wait for the requested session to become available
          // For this we wake-up every time the writer updates the storage

          ret = os.sched.eventWait(getStorage().getEvent());
          if (ret == OSReturn::OS_CANCELLED)
            {
              return ret;
            }

          os.sched.lock.enter();
            {
              // Get the most recently written session from the storage cache
              m_currentSession = getStorage().getMostRecentlyWrittenSession();
            }
          os.sched.lock.exit();

          if (m_currentSession.getUniqueId() == sessionId)
            {
              goto sessionFound;
            }
        }
#endif
    }

  // The storage has content and we were asked for a specific session.
  // Search for the given session id
  SessionUniqueId_t currentSessionUniqueId;

  do
    {
      ret = openPreviousSession();
      if (ret == OSReturn::OS_END_OF_COLLECTION)
        {
          // m_currentSession should remain unchanged if the end of storage
          // was reached
          if (sessionId == OLDEST_SESSSION_UNIQUE_ID)
            return OSReturn::OS_OK;

          // Otherwise inform the caller we could not find the given session id
          return OSReturn::OS_ITEM_NOT_FOUND;
        }
      else if (ret != OSReturn::OS_OK)
        {
          return ret;
        }

      currentSessionUniqueId = m_currentSession.getUniqueId();

      if (currentSessionUniqueId == sessionId)
        {
          sessionFound: // label

          OSDeviceDebug::putString(" openSession() done blks=");
          OSDeviceDebug::putDec(m_currentSession.getFirstBlockNumber());
          OSDeviceDebug::putString("-");
          OSDeviceDebug::putDec(m_currentSession.getLastBlockNumber());
          OSDeviceDebug::putNewLine();

          return OSReturn::OS_OK;
        }
    }
  while (currentSessionUniqueId > sessionId);

  // Otherwise inform the caller we could not find the given session id
  OSDeviceDebug::putString(" ITEM_NOT_FOUND ");
  return OSReturn::OS_ITEM_NOT_FOUND;
}

OSReturn_t
LargeCircularSessionsStorage::Reader::openPreviousSession(void)
{
  OSDeviceDebug::putString(" openPreviousSession() ");

  if (!m_currentSession.isValid())
    {
      OSDeviceDebug::putString(" NOT_INITIALISED ");
      return OSReturn::OS_NOT_INITIALISED;
    }

  LargeCircularSessionsStorage::Session previousSession;

  // Preserve forward reference
  previousSession.setNextSessionFirstBlockNumber(
      m_currentSession.getFirstBlockNumber());

  // From the first block of the current session, go one block before,
  // i.e. the last block of the previous session
  previousSession.setLastBlockNumber(
      getStorage().computeCircularSessionBlockNumber(
          m_currentSession.getFirstBlockNumber(), -1));

  // Read the last block of the previous session
  OSReturn_t ret;
  ret = getStorage().readStorageBlock(previousSession.getLastBlockNumber(),
      m_blockBuffer, 1);
  if (ret != OSReturn::OS_OK)
    return ret;

  // We are now on the last block of the previous session

  // If not valid, return, m_currentSession still points to the
  // oldest session
  if (Header::readMagic(m_blockBuffer) != Header::MAGIC)
    {
      OSDeviceDebug::putString(" END_OF_COLLECTION_(magic1) ");
      return OSReturn::OS_END_OF_COLLECTION;
    }

  previousSession.setUniqueId(Header::readSessionUniqueId(m_blockBuffer));

  previousSession.setFirstBlockNumber(
      Header::readSessionFirstBlockNumber(m_blockBuffer));

  ret = getStorage().readStorageBlock(previousSession.getFirstBlockNumber(),
      m_blockBuffer, 1);
  if (ret != OSReturn::OS_OK)
    return ret;

  // We are now on the first block of the previous session
  // If not valid, return, m_currentSession still points to the
  // oldest session
  if (Header::readMagic(m_blockBuffer) != Header::MAGIC)
    {
      OSDeviceDebug::putString(" END_OF_COLLECTION_(magic2) ");
      return OSReturn::OS_END_OF_COLLECTION;
    }

  // Check if both blocks are in the same session
  if (Header::readSessionUniqueId(m_blockBuffer)
      != previousSession.getUniqueId())
    {
      OSDeviceDebug::putString(" END_OF_COLLECTION_(sid) ");
      return OSReturn::OS_END_OF_COLLECTION;
    }

  previousSession.setLength(
      getStorage().computeCircularSessionLength(
          previousSession.getLastBlockNumber(),
          previousSession.getFirstBlockNumber()));
  previousSession.setCompletelyWritten(true);

  // Make the previous session the current session
  m_currentSession = previousSession;

  OSDeviceDebug::putString(" openPreviousSession() blocks=");
  OSDeviceDebug::putDec(m_currentSession.getFirstBlockNumber());
  OSDeviceDebug::putString("-");
  OSDeviceDebug::putDec(m_currentSession.getLastBlockNumber());
  OSDeviceDebug::putNewLine();

  ret = OSReturn::OS_OK;
  return ret;
}

OSReturn_t
LargeCircularSessionsStorage::Reader::openNextSession(void)
{
  OSDeviceDebug::putString(" openNextSession() ");
  if (!m_currentSession.isValid())
    {
      OSDeviceDebug::putString(" NOT_INITIALISED ");
      return OSReturn::OS_NOT_INITIALISED;
    }

  LargeCircularSessionsStorage::Session nextSession;

  // Set next session begin one block further than current session end
  nextSession.setFirstBlockNumber(
      getStorage().computeCircularSessionBlockNumber(
          m_currentSession.getLastBlockNumber(), 1));

  OSReturn_t ret;
  ret = getStorage().readStorageBlock(nextSession.getFirstBlockNumber(),
      m_blockBuffer, 1);
  if (ret != OSReturn::OS_OK)
    {
      return ret;
    }

  // Check block magic
  if (Header::readMagic(m_blockBuffer) != Header::MAGIC)
    {
      OSDeviceDebug::putString(" END_OF_COLLECTION_(magic) ");
      return OSReturn::OS_END_OF_COLLECTION;
    }

  // If valid, read header from block
  nextSession.setUniqueId(Header::readSessionUniqueId(m_blockBuffer));
  nextSession.setNextSessionFirstBlockNumber(
      Header::readNextSessionFirstBlockNumber(m_blockBuffer));
  if (nextSession.getNextSessionFirstBlockNumber()
      != Header::INVALID_BLOCKNUMBER)
    {
      nextSession.setLastBlockNumber(
          getStorage().computeCircularSessionBlockNumber(
              nextSession.getNextSessionFirstBlockNumber(), -1));

      nextSession.setLength(
          getStorage().computeCircularSessionLength(
              nextSession.getLastBlockNumber(),
              nextSession.getFirstBlockNumber()));
      nextSession.setCompletelyWritten(true);
    }
  else
    {
      nextSession.setLastBlockNumber(Header::INVALID_BLOCKNUMBER);
      nextSession.setLength(0);
    }

  // Make the next session the current session
  m_currentSession = nextSession;

#if true
  OSDeviceDebug::putString(" openNextSession() done id=");
  OSDeviceDebug::putHex(m_currentSession.getUniqueId());
  OSDeviceDebug::putString(", blocks=");
  OSDeviceDebug::putDec(m_currentSession.getFirstBlockNumber());
  OSDeviceDebug::putString("-");
  if (m_currentSession.getLastBlockNumber() != Header::INVALID_BLOCKNUMBER)
    {
      OSDeviceDebug::putDec(m_currentSession.getLastBlockNumber());
    }
  else
    {
      OSDeviceDebug::putChar('?');
    }
  OSDeviceDebug::putNewLine();
#endif

  ret = OSReturn::OS_OK;
  return ret;
}

// Read the requested session block (minimum one device block)
// The block number should be between 0 and (sessionLength-1)
// If the session is not complete, eventually wait for the blocks to
// become available
OSReturn_t
LargeCircularSessionsStorage::Reader::readSessionBlock(
    SessionBlockNumber_t sessionBlockNumber,
    OSDeviceBlock::BlockCount_t deviceBlocksCount, uint8_t* pBlockBuffer,
    bool doNotBlock)
{

  // Add requested sessionBlockNumber to session start
  SessionBlockNumber_t storageBlockNumber;
  storageBlockNumber = getStorage().computeCircularSessionBlockNumber(
      m_currentSession.getFirstBlockNumber(), (int) sessionBlockNumber);

#if defined(OS_DEBUG_LARGECIRCULARSESSIONSSTORAGE_READSESSIONBLOCK)
  OSDeviceDebug::putString(" readSessionBlock() blk=");
  OSDeviceDebug::putDec(sessionBlockNumber);
  OSDeviceDebug::putString(", sblk=");
  OSDeviceDebug::putDec(storageBlockNumber);
  OSDeviceDebug::putString(", len=");
  OSDeviceDebug::putDec(m_currentSession.getLength());
  OSDeviceDebug::putNewLine();
#else
  OSDeviceDebug::putString(" rsb=");
  OSDeviceDebug::putDec(storageBlockNumber);
  OSDeviceDebug::putChar(' ');
#endif /* defined(OS_DEBUG_LARGECIRCULARSESSIONSSTORAGE_READSESSIONBLOCK) */

  OSReturn_t ret;

#if true

  // Set wait parameters
  m_readSessionBlockCondition.setSessionBlockNumber(sessionBlockNumber);

  ret = m_readSessionBlockCondition.wait(getStorage().getEvent(), doNotBlock);
  if (ret != OSReturn::OS_OK)
    return ret;

#else
  for (;;)
    {
      if (m_currentSession.isCompletelyWritten())
        {
          // If the session was completely written, validate block number
          if (sessionBlockNumber >= m_currentSession.getLength())
            {
              OSDeviceDebug::putString(" OUT_OF_RANGE ");
              return OSReturn::OS_OUT_OF_RANGE;
            }
          else
            {
              break;
            }
        }

      // If the session was not completely written, the block that we need
      // might not have been written, so we need to wait for
      // the writer to complete

      Session& storageSession = getStorage().getMostRecentlyWrittenSession();

      bool needUpdate;
      needUpdate = false;

      os.sched.lock.enter();
        {
          if (storageSession.getUniqueId() == m_currentSession.getUniqueId())
            {
              // Update session from the storage cache
              m_currentSession = storageSession;

#if defined(OS_DEBUG_LARGECIRCULARSESSIONSSTORAGE_READSESSIONBLOCK)
              OSDeviceDebug::putString(" updated blocks=");
              OSDeviceDebug::putDec(m_currentSession.getFirstBlockNumber());
              OSDeviceDebug::putString("-");
              OSDeviceDebug::putDec(m_currentSession.getLastBlockNumber());
              OSDeviceDebug::putString(", len=");
              OSDeviceDebug::putDec(m_currentSession.getLength());
              OSDeviceDebug::putNewLine();
#endif /* defined(OS_DEBUG_LARGECIRCULARSESSIONSSTORAGE_READSESSIONBLOCK) */
            }
          else
            {
              needUpdate = true;
            }
        }
      os.sched.lock.exit();

      if (needUpdate)
        {
          // The writer might have closed our session and open another one
          // TODO: update session from storage
          OSDeviceDebug::putString(" BAD_STATE_todo ");
          return OSReturn::OS_BAD_STATE;
        }

      // Here the m_currentSession must be up to date

      SessionBlockNumber_t currentLength;
      currentLength = getStorage().computeCircularSessionLength(
          m_currentSession.getLastBlockNumber(),
          m_currentSession.getFirstBlockNumber());

#if defined(OS_DEBUG_LARGECIRCULARSESSIONSSTORAGE_READSESSIONBLOCK)
      OSDeviceDebug::putString(" clen=");
      OSDeviceDebug::putDec(currentLength);
      OSDeviceDebug::putString(", blk=");
      OSDeviceDebug::putDec(sessionBlockNumber);
      OSDeviceDebug::putNewLine();
#endif /* defined(OS_DEBUG_LARGECIRCULARSESSIONSSTORAGE_READSESSIONBLOCK) */

      if (sessionBlockNumber < currentLength)
        {
          break;
        }

      if (doNotBlock)
        {
          OSDeviceDebug::putString(" WOULD_BLOCK ");
          return OSReturn::OS_WOULD_BLOCK;
        }

      OSEvent_t event;
      event = getStorage().getEvent();

#if defined(OS_DEBUG_LARGECIRCULARSESSIONSSTORAGE_READSESSIONBLOCK)
      OSDeviceDebug::putString(" wait(");
      OSDeviceDebug::putHex(event);
      OSDeviceDebug::putString(") ");
#endif /* defined(OS_DEBUG_LARGECIRCULARSESSIONSSTORAGE_READSESSIONBLOCK) */

      // TODO: use scheduler lock to avoid race condition
      OSEventWaitReturn_t eventWaitReturn;
      eventWaitReturn = os.sched.eventWait(event);

#if defined(OS_DEBUG_LARGECIRCULARSESSIONSSTORAGE_READSESSIONBLOCK)
      OSDeviceDebug::putString(" eventWait=");
      OSDeviceDebug::putHex(eventWaitReturn);
      OSDeviceDebug::putString(" ");
#endif /* defined(OS_DEBUG_LARGECIRCULARSESSIONSSTORAGE_READSESSIONBLOCK) */

      if (eventWaitReturn == OSEventWaitReturn::OS_CANCELLED)
        {
          OSDeviceDebug::putString(" CANCELLED ");
          return OSReturn::OS_CANCELLED;
        }
    }
#endif

  ret = getStorage().readStorageBlock(storageBlockNumber, pBlockBuffer,
      deviceBlocksCount);
  if (ret != OSReturn::OS_OK)
    {
      return ret;
    }

  Magic_t magic;
  magic = LargeCircularSessionsStorage::Header::readMagic(pBlockBuffer);

  // Check block magic
  if (magic != LargeCircularSessionsStorage::Header::MAGIC)
    {
      OSDeviceDebug::putString(" BAD_STATE_(magic) ");
      return OSReturn::OS_BAD_STATE;
    }

  // Check session unique id
  SessionUniqueId_t sessionUniqueId;
  sessionUniqueId = LargeCircularSessionsStorage::Header::readSessionUniqueId(
      pBlockBuffer);

  if (sessionUniqueId != m_currentSession.getUniqueId())
    {
      // If the session id changes, return eof
      OSDeviceDebug::putString(" BAD_STATE_(sid) ");
      ret = OSReturn::OS_BAD_STATE;
    }

#if defined(OS_DEBUG_LARGECIRCULARSESSIONSSTORAGE_READSESSIONBLOCK)
  OSDeviceDebug::putString(" readSessionBlock() done ");
#else
  OSDeviceDebug::putString(" rsbDone ");
#endif /* defined(OS_DEBUG_LARGECIRCULARSESSIONSSTORAGE_READSESSIONBLOCK) */
  return OSReturn::OS_OK;
}

OSReturn_t
LargeCircularSessionsStorage::Reader::closeSession(void)
{
  OSDeviceDebug::putString(" closeSession()");
  OSDeviceDebug::putNewLine();

  return OSReturn::OS_OK;
}

// ============================================================================

// ----- Constructors & destructors -------------------------------------------

LargeCircularSessionsStorage::Reader::ReadCondition::ReadCondition(
    Reader& reader) :
  m_reader(reader)
{
  OSDeviceDebug::putConstructor_P(
      PSTR("LargeCircularSessionsStorage::Reader::ReaderCondition"), this);
}

LargeCircularSessionsStorage::Reader::ReadCondition::~ReadCondition()
{
  OSDeviceDebug::putDestructor_P(
      PSTR("LargeCircularSessionsStorage::Reader::ReaderCondition"), this);
}

// ----- Public methods -------------------------------------------------------

OSReturn_t
LargeCircularSessionsStorage::Reader::ReadCondition::prepareCheckCondition(void)
{
  // We first check the current reader session data, and if the session
  // is complete, we gladly return from the wait().

  LargeCircularSessionsStorage::Session& currentSession =
      getReader().getCurrentSession();

  if (currentSession.isCompletelyWritten())
    {
      // If the session was completely written, validate block number
      if (m_sessionBlockNumber >= currentSession.getLength())
        {
          OSDeviceDebug::putString(" OUT_OF_RANGE ");
          return OSReturn::OS_OUT_OF_RANGE;
        }
      else
        {
          //OSDeviceDebug::putString(" available ");
          // We can proceed and read the block, it must be there
          return OSReturn::OS_OK;
        }
    }

  //OSDeviceDebug::putString(" not complete ");

  // If the session was not completely written, the block that we need
  // might have not been written, so we need to wait for
  // the writer to complete

  Session& storageSession =
      getReader().getStorage().getMostRecentlyWrittenSession();

  if (storageSession.getUniqueId() != currentSession.getUniqueId())
    {
      // The writer might have closed our session and open another one.

      // TODO: update session from storage (we can do it here, this
      // code has no restrictions related to system calls)

      OSDeviceDebug::putString(" BAD_STATE_todo ");
      return OSReturn::OS_BAD_STATE;
    }

  return OSReturn::OS_SHOULD_WAIT;
}

// WARNING: runs within a scheduler critical section

OSReturn_t
LargeCircularSessionsStorage::Reader::ReadCondition::checkSynchronisedCondition(
    void)
{
  LargeCircularSessionsStorage::Session& currentSession =
      getReader().getCurrentSession();

  Session& storageSession =
      getReader().getStorage().getMostRecentlyWrittenSession();

  if (storageSession.getUniqueId() != currentSession.getUniqueId())
    {
      // The writer might have closed our session and open another one. This
      // means that the condition we are checking here is not stable, and
      // we need to retry.
      // This will make the flow go back to prepareCheckCondition()
      // where we need to read the session data from storage.

      OSDeviceDebug::putString(" SHOULD_RETRY ");
      return OSReturn::OS_SHOULD_RETRY;
    }

  // We checked that we are waiting on a live session, so
  // we can update the session info from the storage cache.

  // This is a structure copy, but we are not concerned, since this
  // runs in a critical section.

  // currentSession being a reference, this will actually update
  // the Reader instance.

  currentSession = storageSession;

#if defined(OS_DEBUG_LARGECIRCULARSESSIONSSTORAGE_READSESSIONBLOCK)
  OSDeviceDebug::putString(" updated blocks=");
  OSDeviceDebug::putDec(currentSession.getFirstBlockNumber());
  OSDeviceDebug::putString("-");
  OSDeviceDebug::putDec(currentSession.getLastBlockNumber());
  OSDeviceDebug::putString(", len=");
  OSDeviceDebug::putDec(currentSession.getLength());
  OSDeviceDebug::putNewLine();
#endif /* defined(OS_DEBUG_LARGECIRCULARSESSIONSSTORAGE_READSESSIONBLOCK) */

  // Here the Reader m_currentSession must be up to date.
  // We can compute the current session length.

  SessionBlockNumber_t currentLength;
  currentLength
      = getReader().getStorage().computeCircularSessionLength(
          currentSession.getLastBlockNumber(),
          currentSession.getFirstBlockNumber());

#if defined(OS_DEBUG_LARGECIRCULARSESSIONSSTORAGE_READSESSIONBLOCK)
  OSDeviceDebug::putString(" clen=");
  OSDeviceDebug::putDec(currentLength);
  OSDeviceDebug::putString(", blk=");
  OSDeviceDebug::putDec(m_sessionBlockNumber);
  OSDeviceDebug::putNewLine();
#endif /* defined(OS_DEBUG_LARGECIRCULARSESSIONSSTORAGE_READSESSIONBLOCK) */

  // This is the actual condition that we are checking, i.e. if the
  // requested block was written and is available for read.

  if (m_sessionBlockNumber < currentLength)
    {
      // If so, make the wait return
      return OSReturn::OS_OK;
    }

  // We did our best, but the block is not yet available,
  // we'll wait for the writer to notify us after each writeSessioBlock()
  // or closeSession().

  return OSReturn::OS_SHOULD_WAIT;
}

// ----------------------------------------------------------------------------

// ============================================================================

#endif /* defined(OS_INCLUDE_LARGECIRCULARSESSIONSSTORAGE) */
