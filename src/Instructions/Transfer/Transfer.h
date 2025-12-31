#ifndef TRANSFER
#define TRANSFER

#include "HasAdressMode.h"
#include "HasFlags.h"
#include "Implied.h"

class Transfer : public HasAdressMode, public HasFlags
{
private:
    shared_ptr<Implied> mDest;

public:
    Transfer(shared_ptr<Implied> dest, shared_ptr<Implied> adressMode, Flags *fl) : HasAdressMode(adressMode), HasFlags(fl)
    {
        mDest = dest;
    }

    void execute()
    {
        mFlags->Zero = (mAdressMode->getValue() == 0);
        mFlags->Negative = (mAdressMode->getValue() >> 7);
        mDest->setValue(mAdressMode->getValue());
        // *mDest->getResult() = *mAdressMode->getResult();
    }
};

#endif