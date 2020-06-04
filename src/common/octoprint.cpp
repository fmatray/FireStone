#include "common/octoprint.h"

#include "common/debug.h"
#include "common/helpers.h"
#include "config.h"
/*
    SETUP
*/

void OctoprintBase::begin() {
  api = new OctoprintApi(client, OCTOPRINT_IP, OCTOPRINT_PORT, OCTOPRINT_APIKEY);
#ifdef FS_DEBUG
  api->_debug = true;
#endif
}