#include "smsmanager.h"
#include <gnokii.h>
#include <QDebug>

// http://git.savannah.gnu.org/cgit/gnokii/gnokii-extras.git/tree/snippets/sms/sms_status.c

SMSManager::SMSManager(QObject *parent) : QObject(parent) {

}

SMSManager::~SMSManager()
{
    gn_lib_phone_close(state);
    gn_lib_phoneprofile_free(&state);
    gn_lib_library_free();
}

void SMSManager::openCellular()
{
    qDebug() << Q_FUNC_INFO << "Initializing gnokii " << gn_lib_version();
    state = 0;
    gn_error error = gn_lib_init();
    if(error != GN_ERR_NONE){
        qDebug() << Q_FUNC_INFO << "error initializing gnokii";
        return;
    }
    error = gn_lib_phoneprofile_load(NULL, &state);
    if(error != GN_ERR_NONE) {
        qDebug() << Q_FUNC_INFO << "error loading phone profile:" << gn_error_print(error);
        return;
    }
    qDebug() << Q_FUNC_INFO << "phone opened successfully";
}

void SMSManager::getRfLevel() {
    gn_error error;
    gn_data data;
    float rfLevel = 0;
    gn_rf_unit rfUnit;
    gn_data_clear(&data);
    data.rf_level = &rfLevel;
    data.rf_unit = &rfUnit;

    error = gn_sm_functions(GN_OP_GetRFLevel, &data, state);
    if (error == GN_ERR_NONE) {
        qDebug() << Q_FUNC_INFO << "RF Level: " << rfLevel;
    } else {
        qDebug() << Q_FUNC_INFO << "error getting RF level:" << gn_error_print(error);
    }
}

void SMSManager::sendSms(QString number, QString message, bool simulate)
{
    gn_error error;
    gn_data data;
    gn_sms sms;
    gn_sms_default_submit(&sms);
    gn_data_clear(&data);
    memset(&sms.remote.number, 0, sizeof(sms.remote.number));
    strncpy(sms.remote.number, number.toAscii(), sizeof(sms.remote.number) - 1);
    if (sms.remote.number[0] == '+')
            sms.remote.type = GN_GSM_NUMBER_International;
    else
            sms.remote.type = GN_GSM_NUMBER_Unknown;

    sms.delivery_report = true;

    char message_buffer[255 * GN_SMS_MAX_LENGTH];
    strcpy(message_buffer, message.toAscii());
    strcpy((char*) sms.user_data[0].u.text, message_buffer);

    if (!sms.smsc.number[0]) {
            data.message_center = (gn_sms_message_center *) calloc(1, sizeof(gn_sms_message_center));
            data.message_center->id = 1;
            if (gn_sm_functions(GN_OP_GetSMSCenter, &data, state) == GN_ERR_NONE) {
                    strcpy(sms.smsc.number, data.message_center->smsc.number);
                    sms.smsc.type = data.message_center->smsc.type;
            }
            free(data.message_center);
    }

    if (!sms.smsc.type) sms.smsc.type = GN_GSM_NUMBER_Unknown;

    sms.user_data[0].type = GN_SMS_DATA_Text;
    if (!gn_char_def_alphabet(sms.user_data[0].u.text))
            sms.dcs.u.general.alphabet = GN_SMS_DCS_UCS2;
    sms.user_data[1].type = GN_SMS_DATA_None;

    data.sms = &sms;
    error = gn_sms_send(&data, state);

    if (error == GN_ERR_NONE) {
        qDebug() << Q_FUNC_INFO << "SMS Sent";
    } else {
        qDebug() << Q_FUNC_INFO << "error sending sms:" << gn_error_print(error);
    }
}
