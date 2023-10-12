#ifndef _ZIC_ENVELOP_H_
#define _ZIC_ENVELOP_H_

#include <vector>

class Envelop {
protected:
    unsigned int index = 0;

public:
    struct Data {
        float modulation;
        float time;
    };

    std::vector<Data> data;

    Envelop(std::vector<Data> data)
        : data(data)
    {
    }

    float next(float time)
    {
        if (index > data.size() - 1) {
            return 0.0f;
        }

        if (time >= data[index + 1].time) {
            index++;
        }
        float timeOffset = data[index + 1].time - data[index].time;
        float timeRatio = (time - data[index].time) / timeOffset;
        return (data[index + 1].modulation - data[index].modulation) * timeRatio + data[index].modulation;
    }

    void reset()
    {
        index = 0;
    }
};

#endif
