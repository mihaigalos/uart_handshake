#include "osi_layers/transport.h"

#include "utilities.h"

template <>
CommunicationStatus Transport<>::Transmit(uint8_t to, uint8_t *data, uint32_t size) const
{
    auto result{CommunicationStatus::Unknown};
    TSegment segment{};

    for (uint32_t i = 0; i < size;)
    {
        uint8_t j{0};
        Payload payload{};
        for (j = 0; j <= (kPayloadMaxSize - kSizeOfToField - kSizeOfFromField - kCRCSize - sizeof(TSegment)) && (i + j <= size); ++j)
        {
            payload.data[j] = data[i + j];
        }
        payload.size = j - 1;
        i += j - 1;

        payload.data[payload.size++] = static_cast<uint8_t>(segment);
        payload.data[payload.size++] = static_cast<uint8_t>(segment >> 8);
        log_dump_payload(payload, std::string{"Transport :: Transmit ["} + std::to_string(segment) + "]");
        ++segment;

        result = network_.Transmit(to, payload);

        if (result != CommunicationStatus::Acknowledge)
        {
            break;
        }
    }

    return result;
}

template <>
Payload Transport<>::Receive(uint8_t from) const
{
    static_cast<void>(from);
    return {};
}