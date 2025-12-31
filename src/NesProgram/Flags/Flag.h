#ifndef FLAG
#define FLAG

class Flag
{
protected:
    bool mFlag;

public:
    void setFlag(bool flag) noexcept
    {
        mFlag = flag;
    }

    bool getFlag() const
    {
        return mFlag;
    }

    void conditon();
};

#endif