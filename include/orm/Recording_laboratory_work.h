#ifndef INCLUDE_ORM_RECORDING_LABORATORY_WORK_H_
#define INCLUDE_ORM_RECORDING_LABORATORY_WORK_H_

#include "Wrapper.h"

class Recording_laboratory_work {
 public:
    explicit Recording_laboratory_work(
            int recording_id,
            int subject_id,
            time_t date_lab,
            int group_id,
            bool _deleted) :
            id(recording_id),
            subject(subject_id),
            date_lab(date_lab),
            group(group_id),
            deleted(_deleted) {}
    static int AddRecording(SQLWrapper &db, int subject_id, time_t date_lab, int group_id);
    static void DeleteRecording(SQLWrapper &db, int record_id);
    static void UpdateRecording(SQLWrapper &db, int recording_id, int user_id, int subject_id,
            time_t date_lab, int group_id, unsigned int user_position);
    static Recording_laboratory_work GetRecording(SQLWrapper &db, int user_id, int subject_id);

    int id;
    time_t date_lab;
    int subject;
    int group;
    bool deleted;
};

#endif  // INCLUDE_ORM_RECORDING_LABORATORY_WORK_H_
