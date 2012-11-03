#include "smsmanager.h"
#include <gnokii.h>
#include <QDebug>

// http://git.savannah.gnu.org/cgit/gnokii/gnokii-extras.git/tree/snippets/sms/sms_status.c

SMSManager::SMSManager(QObject *parent) : QObject(parent) {
    qDebug() << Q_FUNC_INFO << "Initializing gnokii " << gn_lib_version();
    if(!gn_lib_init() == GN_ERR_NONE){
        qDebug() << Q_FUNC_INFO << "error initializing";
    }
}

SMSManager::~SMSManager()
{
    gn_lib_library_free();
}

void SMSManager::openCellular()
{
}
