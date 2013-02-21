#include "include/identity.h"

class TaskIdentity : public Identity {
};

typedef Singleton<TaskIdentity> TaskIdentityI;
