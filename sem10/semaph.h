#define TRY(FUNC)                       \
        if(FUNC < 0)                    \
        {                               \
                perror("Error" #FUNC);  \
                exit(-1);               \
        }                               \

void V(int sem_id, enum semaphors sem_num, int num)
{
        struct sembuf sem_op = {sem_num, num, 0};
        TRY(semop(sem_id, &sem_op, 1));
}

void P(int sem_id, enum semaphors sem_num, int num)
{
        num = -num;
        struct sembuf sem_op = {sem_num, num, 0};
        TRY(semop(sem_id, &sem_op, 1));
}

void Z(int sem_id, enum semaphors sem_num)
{
        struct sembuf sem_op = {sem_num, 0, 0};
        TRY(semop(sem_id, &sem_op, 1));
}
