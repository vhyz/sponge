#include <SpongeBob/Logger.h>

int main() {
    setLogLevel(LOG_LEVEL_ERROR);
    DEBUG("%s", "DEBUG");
    INFO("%s", "INFO");
    WARN("%s", "WARN");
    FATAL("%s", "FATAL");
    ERROR("%s", "ERROR");
}