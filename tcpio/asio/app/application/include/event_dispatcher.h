#ifndef ACC10646_E5D7_4615_BD39_6B2AABB877E5
#define ACC10646_E5D7_4615_BD39_6B2AABB877E5

#include <stdint.h>
#include "event_msg.h"

class EventDispatcher
{
public:
    static void ProcessMessage(const EventMessage& msg);
};

#endif /* ACC10646_E5D7_4615_BD39_6B2AABB877E5 */
