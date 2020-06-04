#include "common/octoprint.h"

#include "common/debug.h"
#include "common/helpers.h"
#include "config.h"
/*
    SETUP
*/

void OctoprintBase::begin() {
  title("Octoprint Setup");
  api = new OctoprintApi(client, OCTOPRINT_IP, OCTOPRINT_PORT, OCTOPRINT_APIKEY);
  client.setConnectTimeout(500);
#ifdef FS_DEBUG
  api->_debug = true;
#endif
}