#include "baseband.h"

using namespace rd;
using namespace std;

bool baseband::connect(const list<pci_dev *> &ctrller)
{
    list<pci_dev *>::const_iterator iter;
    for (iter = ctrller.begin();iter != ctrller.end();iter ++) {
        _ctrller_pci.push_back(*iter);
    }
    return true;
}
