#ifndef QLINEFREQEDIT_H
#define QLINEFREQEDIT_H

#include <QLineEdit>
#include <list>
#include <QStateMachine>

using namespace std;

class QLineFreqEdit : public QLineEdit
{
public:
	explicit QLineFreqEdit(QWidget* parent=0);
	~QLineFreqEdit();
	int getFreqs(list<double> &freqs);
};

#endif // QLINEFREQEDIT_H
