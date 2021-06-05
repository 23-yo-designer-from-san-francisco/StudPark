#include "Wrapper.h"
#include "Recording_laboratory_work.h"

int Recording_laboratory_work::AddRecording(SQLWrapper &db, int subject_id, time_t date_lab, int group_id) {
    db << "INSERT INTO recording_laboratory_work (subject, date_lab, group_id) values ("
       << subject_id << ", to_timestamp("
       << date_lab << "), "
       << group_id << ") returning id;";
    db.exec();

    return db.get_int(0);
}

void Recording_laboratory_work::DeleteRecording(SQLWrapper &db, int record_id) {
    db << "DELETE FROM recording_laboratory_work WHERE id = " << record_id << ";";
    db.exec();
}
