# syncthing service definition
SERVICE_COMMAND="${SYNOPKG_PKGDEST}/bin/syncthing -home=${SYNOPKG_PKGVAR}"
SVC_BACKGROUND=y
SVC_WRITE_PID=y

SERVICE_COMMAND="${SYNOPKG_PKGDEST}/bin/syncthing --home=${SYNOPKG_PKGVAR}"
SVC_BACKGROUND=y
SVC_WRITE_PID=y

GROUP="sc-syncthing"

service_prestart ()
{
<<<<<<< HEAD
    CONFIG_DIR="${SYNOPKG_PKGVAR}"
    SYNCTHING_OPTIONS="-home=${CONFIG_DIR}"

    # Read additional startup options from /usr/local/syncthing/var/options.conf
    if [ -f ${CONFIG_DIR}/options.conf ]; then
        . ${CONFIG_DIR}/options.conf
    fi

    SERVICE_OPTIONS=$SYNCTHING_OPTIONS

    # Required: start-stop-daemon do not set environment variables
=======
    # Read additional startup options from var/options.conf
    if [ -f ${SYNOPKG_PKGVAR}/options.conf ]; then
        . ${SYNOPKG_PKGVAR}/options.conf
        SERVICE_COMMAND="${SERVICE_COMMAND} ${SYNCTHING_OPTIONS}"
    fi

    # Required: set $HOME environment variable
>>>>>>> d44affc1a5e8ba9a78392acb39f2e45161c48f9b
    HOME=${SYNOPKG_PKGVAR}
    export HOME
}
