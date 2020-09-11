#include "QLineFreqEdit.h"
#include <QMessageBox>

QLineFreqEdit::QLineFreqEdit(QWidget *parent) :
	QLineEdit(parent)
{
	setToolTip("<html><head/><body><p><span style= font-weight:600;>eg. </span></p> \
		<p>400;600;800; // means <span style=font-weight:600;color:#0000ff;>400MHz</span> and <span style= font-weight:600;color:#0000ff;>600MHz</span></p> \
		<p>400~6000:50; // means from <span style=font-weight:600;color:#0000ff;>400MHz</span> to <span style=font-weight:600;color:#0000ff;>6GHz</span> step <span style=font-weight:600;color:#0000ff;>50MHz</span></p> \
		<p>400~1000:10;1200; // means from <span style= font-weight:600;color:#0000ff;>400MHz </span>to <span style=font-weight:600;color:#0000ff;>1GHz </span>step <span style=font-weight:600;color:#0000ff;>10MHz</span> and <span style=font-weight:600;color:#0000ff;>1.2G</span></p> \
		</body></html>");
	setText("400;500-1000:10;");
}

QLineFreqEdit::~QLineFreqEdit()
{

}

int QLineFreqEdit::getFreqs(list<double> &freqs) {

	freqs.clear();

	QByteArray bytes = this->text().toUtf8().constData();
	char buf[256];
	int index;
	int len = bytes.count();
	double min = 0, max = 0, step = 1;
	int i;
	enum states { point, range_max, range_step };
	enum errors { no_error, error_max, error_step, error_character };

	const char errstring[][64] = { "", "Range Max Input Error", "Step Input Error", "Unrecognize Character"};

	states state = point;
	errors err = no_error;

	if (len == 0)
		return 0;

	if (bytes[bytes.count() - 1] != ';')
		bytes.append(';');
	this->setText(QString(bytes));

    memset(buf,0,sizeof(buf));
	index = 0;

	for (i = 0; i < len; i++) {
		switch (bytes[i]) {
			case ';':
				switch (state) {
					case point:
						freqs.push_back(atof(buf));

                        memset(buf,0,sizeof(buf));
						index = 0;
						break;
					case range_step:
						step = atof(buf);
						for (double d = min; d < max + 1e-6; d += step)
                            freqs.push_back(d);
                        memset(buf,0,sizeof(buf));
						index = 0;
						break;
					case range_max:
						err = error_step;
						break;
				}
				state = point;
				break;
			case ':':
				switch (state) {
					case point:
						err = error_max;
						break;
					case range_max:
						max = atof(buf);
                        memset(buf,0,sizeof(buf));
						index = 0;
						state = range_step;
						break;
					case range_step:
						err = error_step;
						break;
				}
				break;

			case '~':
				switch (state) {
					case point:
                    min = atof(buf);
                    memset(buf,0,sizeof(buf));
                    index = 0;
						state = range_max;
						break;
					case range_max:
						err = error_max;
						break;
					case range_step:
						err = error_step;
						break;
				}
				break;

			default:
				if ((bytes[i] < '0' || bytes[i] > '9') && bytes[i] != '.' && bytes[i] != '-') {
					err = error_character;
					break;
				}
				buf[index++] = bytes[i];
				break;
		}

		if (err != no_error) {
			QMessageBox msgBox(QMessageBox::Critical, "Input Error", QString("Input Error @ %1, Message : %2").arg(i).arg(errstring[err]), QMessageBox::Ok);
			setSelection(i, 1);
			msgBox.exec();
			setFocus();
			return -1;
		}
	}
	freqs.sort();
	return 0;

}

