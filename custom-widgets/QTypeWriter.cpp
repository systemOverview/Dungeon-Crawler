#include "QTypeWriter.h"

QTypeWriter::QTypeWriter() {ensureCursorVisible();}

void QTypeWriter::setText(const QString &text)
{
    QTextEdit::setText("");
    for (int i =0; i<text.length(); i++){
        auto cursor = textCursor();
        cursor.insertText(text[i]);
        ensureCursorVisible();
        Utilities::QtSleepMilliSeconds(SleepTimeBetweenCharactersInMilliSeconds);
    }

}

void QTypeWriter::setSleepTimeBetweenCharacters(int sleepTimeInMilliSeconds)
{
    SleepTimeBetweenCharactersInMilliSeconds = sleepTimeInMilliSeconds;
}
