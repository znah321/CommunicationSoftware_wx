/* Copyright 2017 The MathWorks, Inc. */

#ifndef StreamingProfilerTgtAppSvc_hpp
#define StreamingProfilerTgtAppSvc_hpp

#include <climits>
#include "StreamingProfilerTgtAppSvc_dll.hpp"
#include "coder/target_services/Message.hpp"
#include "coder/target_services/Application.hpp"
#ifdef BUILDING_LIBMWSOC_STREAMINGPROFILERTGTAPPSVC
#  include "coder/target_services/CommService.hpp"
#else
#  include "CommService.hpp"
#endif
#include "coder/target_services/fifo.hpp"

#ifndef PROFILER_MAX_RX_BUFFER_SIZE
#define PROFILER_MAX_RX_BUFFER_SIZE 100
#endif

class STREAMINGPROFILERTGTAPPSVC_API StreamingProfilerTgtAppSvc : public coder::tgtsvc::Application
{
public:
	virtual ~StreamingProfilerTgtAppSvc();
    enum RequestType {
        INITIALIZE_REQUEST = 0x80
    };
   
    void handleMessage(coder::tgtsvc::Message *message);
   
    uint8_t id() { return(coder::tgtsvc::Application::STREAMING_PROFILER_ID); }

    void initialize(const uint16_t &aTimerBitsPerMessage, const uint16_t &aTimerBitness);

	virtual void handleConnect(bool connected)
    {
        if (connected)
        {
            sendWaitingMessages();
        }
    };

    void uploadProfileData(const void *aTaskID, const void *aCoreID, const void *aTimerValue);
   
    static StreamingProfilerTgtAppSvc & get_instance();

private:
    enum {
        RX_BUFFER_SIZE = PROFILER_MAX_RX_BUFFER_SIZE,
    };

    enum MessageType {
        DISTRESS_MESSAGE = 0xE0,
        FIRST_MESSAGE = 0x80,
        MIDDLE_MESSAGE = 0x40,
        END_MESSAGE = 0xC0,
        INITIALIZE_MESSAGE = 0x00
    };

    StreamingProfilerTgtAppSvc();
    StreamingProfilerTgtAppSvc(const StreamingProfilerTgtAppSvc &);           
	const StreamingProfilerTgtAppSvc& operator=(StreamingProfilerTgtAppSvc &);

    bool addToQueue(coder::tgtsvc::Message *bufPtr)
    {
        if (bufPtr != NULL && !fProfileQueue_.full())
        {
            fProfileQueue_.push(bufPtr);
            return true;
        }
        else
        {
            return false;
        }
    }
private:
   
    bool send(coder::tgtsvc::Message *msg);
   
    void sendWaitingMessages();
    coder::tgtsvc::detail::fifo<coder::tgtsvc::Message*, RX_BUFFER_SIZE> fProfileQueue_;
    size_t fBytesSentInLastMessage_;
    volatile bool fInitialized_;
    volatile bool fInitializationSent_;
    volatile bool fErrorState_;
    volatile bool fSendingMessages_;
    volatile bool fBusy_;

    uint16_t fTimerBitsPerMessage_;
    uint16_t fBitnessOfTimer_;
    uint16_t fWordSize_;
    bool fIsBigEndian_;
};
#endif
