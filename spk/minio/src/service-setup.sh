# Setup environment
PATH="${SYNOPKG_PKGDEST}/bin:${PATH}"
GROUP="sc-minio"

INST_ETC="/var/packages/${SYNOPKG_PKGNAME}/etc"
INST_VARIABLES="${INST_ETC}/installer-variables"

service_postinst ()
{
    echo "WIZARD_DATA_VOLUME=${wizard_data_volume}" >> ${INST_VARIABLES}
    echo "WIZARD_DATA_DIRECTORY=${wizard_data_directory}" >> ${INST_VARIABLES}
    echo "WIZARD_ROOT_USER=${wizard_root_user}" >> ${INST_VARIABLES}
    echo "WIZARD_ROOT_PASSWORD=${wizard_root_password}" >> ${INST_VARIABLES}
}

if [ -f ${INST_VARIABLES} ]; then
  . ${INST_VARIABLES}
fi

export MINIO_ROOT_USER=$WIZARD_ROOT_USER
export MINIO_ROOT_PASSWORD=$WIZARD_ROOT_PASSWORD
export HOME=${SYNOPKG_PKGVAR}

<<<<<<< HEAD
    # Required: start-stop-daemon do not set environment variables
    HOME=${SYNOPKG_PKGVAR}
    export HOME
}
=======
MINIO="${SYNOPKG_PKGDEST}/bin/minio"
SERVICE_COMMAND="${MINIO} server --quiet --anonymous ${WIZARD_DATA_VOLUME}/${WIZARD_DATA_DIRECTORY}"
SVC_BACKGROUND=y
SVC_WRITE_PID=y
>>>>>>> d44affc1a5e8ba9a78392acb39f2e45161c48f9b
