#include "rwlock.h"

int rwlock_init(rwlock_t* rwl)
{
	int status;

	rwl->r_active = 0;
	rwl->w_active = 0;
	rwl->r_wait = 0;
	rwl->w_wait = 0;

	status = pthread_mutex_init(&rwl->mutex, NULL);
	if(status)
	{
		return status;
	}
	status = pthread_cond_init(&rwl->read, NULL);
	if(status)
	{
		pthread_mutex_destroy(&rwl->mutex);
		return status;
	}
	status = pthread_cond_init(&rwl->write, NULL);
	if(status)
	{
		pthread_mutex_destroy(&rwl->mutex);
		pthread_cond_destroy(&rwl->read);
		return status;
	}
	return 0;
}

static void rwl_readcleanup (void* rwl_v)
{
	rwlock_t* rwl = (rwlock_t *) rwl_v;

	rwl->r_wait--;
	pthread_mutex_unlock(&rwl->mutex);
}

int rwlock_readlock(rwlock_t* rwl)
{
	int status;

	status = pthread_mutex_lock(&rwl->mutex);
	if(status != 0)
	{
		return -1;
	}
	if(rwl->w_active)
	{
		rwl->r_wait++;
		pthread_cleanup_push(rwl_readcleanup, (void *)rwl);
		while(rwl->w_active)
		{
			status = pthread_cond_wait(&rwl->read, &rwl->mutex);
			if(status)
			{
				break;
			}
		}
		pthread_cleanup_pop(0);
		rwl->r_wait--;
	}
	if(!status)
	{
		rwl->r_active++;
	}
	pthread_mutex_unlock(&rwl->mutex);
	return status;
}

static void rwl_writecleanup (void* rwl_v)
{
	rwlock_t* rwl = (rwlock_t *) rwl_v;

	rwl->w_wait--;
	pthread_mutex_unlock(&rwl->mutex);
}

int rwlock_writelock(rwlock_t* rwl)
{
	int status;

	status = pthread_mutex_lock(&rwl->mutex);
	if(status != 0)
 	{
 		return status;
	}
	if(rwl->w_active || rwl->r_active > 0)
	{
		rwl->w_wait++;
                pthread_cleanup_push(rwl_writecleanup, (void *)rwl);
                while(rwl->w_active || rwl->r_active > 0)
                {
                        status = pthread_cond_wait(&rwl->write, &rwl->mutex);
                        if(status)
                        {
                                break;
                        }
                }
                pthread_cleanup_pop(0);
                rwl->w_wait--;
        }
        if(!status)
        {
                rwl->w_active = 1;
        }
        pthread_mutex_unlock(&rwl->mutex);
        return status;
}

int rwlock_readunlock(rwlock_t* rwl)
{
	int status;

	status = pthread_mutex_lock(&rwl->mutex);
	if(status)
	{
		return status;
	}
	rwl->r_active--;
	if(rwl->r_active == 0 && rwl->w_wait > 0)
	{
		status = pthread_cond_signal(&rwl->write);
		if(status)
		{
			return status;
		}
	}
	status = pthread_mutex_unlock(&rwl->mutex);
	return status;
}

int rwlock_writeunlock(rwlock_t* rwl)
{
        int status;

        status = pthread_mutex_lock(&rwl->mutex);
        if(status)
        {
                return status;
        }
        rwl->w_active = 0;
        if(rwl->r_wait > 0)
        {
                status = pthread_cond_broadcast(&rwl->write);
                if(status)
                {
			pthread_mutex_unlock(&rwl->mutex);
                        return status;
                }
        } else if(rwl->w_wait > 0)
	{
		status = pthread_cond_signal(&rwl->write);
		if(status)
		{
			pthread_mutex_unlock(&rwl->mutex);
			return status;
		}
	}
        status = pthread_mutex_unlock(&rwl->mutex);
        return status;
}

int rwlock_destroy(rwlock_t* rwl)
{
	int status;
	
	status = pthread_mutex_lock(&rwl->mutex);
	if(status)
	{
		return status;
	}
	if(rwl->r_active > 0 || rwl->w_active)
	{
		pthread_mutex_unlock(&rwl->mutex);
		return status;
	}
	if(rwl->r_wait > 0 || rwl->w_wait != 0)
	{
		pthread_mutex_unlock(&rwl->mutex);
		return status;
	}
	status = pthread_mutex_unlock(&rwl->mutex);
	if(status)
	{
		return status;
	}
	status = pthread_cond_destroy(&rwl->read);
	if(status)
	{
		return status;
	}
	status = pthread_cond_destroy(&rwl->write);
	return status;
}
