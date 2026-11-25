#ifndef HAPTICBUTTONEVENT_H
#define HAPTICBUTTONEVENT_H

#include <QEvent>

class HapticButtonEvent : public QEvent
{
    public:
        HapticButtonEvent();

        virtual ~HapticButtonEvent();

        static const QEvent::Type EVENT_TYPE;
};

#endif // HAPTICBUTTONEVENT_H
