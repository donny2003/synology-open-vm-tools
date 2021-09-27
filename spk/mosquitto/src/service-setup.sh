<<<<<<< HEAD
=======
# mosquitto service definition

>>>>>>> d44affc1a5e8ba9a78392acb39f2e45161c48f9b
CFG_FILE="${SYNOPKG_PKGVAR}/mosquitto.conf"
PATH="${SYNOPKG_PKGDEST}/bin:${PATH}"
SERVICE_COMMAND="${SYNOPKG_PKGDEST}/sbin/mosquitto -d -c ${CFG_FILE}"

