PYTHON_DIR="/var/packages/python/target/bin"
GIT_DIR="/var/packages/git/target/bin"
PATH="${SYNOPKG_PKGDEST}/bin:${SYNOPKG_PKGDEST}/env/bin:${PYTHON_DIR}:${GIT_DIR}:${PATH}"
PYTHON="${SYNOPKG_PKGDEST}/env/bin/python"
<<<<<<< HEAD
VIRTUALENV="${PYTHON_DIR}/bin/virtualenv"
GIT="${GIT_DIR}/bin/git"
COUCHPOTATOSERVER="${SYNOPKG_PKGVAR}/CouchPotatoServer/CouchPotato.py"
CFG_FILE="${SYNOPKG_PKGVAR}/settings.conf"
=======
VIRTUALENV="${PYTHON_DIR}/virtualenv"
GIT="${GIT_DIR}/git"
COUCHPOTATOSERVER="${SYNOPKG_PKGDEST}/var/CouchPotatoServer/CouchPotato.py"
CFG_FILE="${SYNOPKG_PKGDEST}/var/settings.conf"
>>>>>>> d44affc1a5e8ba9a78392acb39f2e45161c48f9b

GROUP="sc-download"
LEGACY_GROUP="sc-media"

SERVICE_COMMAND="${PYTHON} ${COUCHPOTATOSERVER} --daemon --pid_file ${PID_FILE} --config_file ${CFG_FILE}"

validate_preinst ()
{
    # Check fork
    if [ ! ${GIT} ls-remote --heads --exit-code ${wizard_fork_url:=git://github.com/CouchPotato/CouchPotatoServer.git} ${wizard_fork_branch:=master} > /dev/null 2>&1; then
        echo "Incorrect fork"
        exit 1
    fi
}

service_postinst ()
{
    # Create a Python virtualenv
    ${VIRTUALENV} --system-site-packages ${SYNOPKG_PKGDEST}/env

    if [ "${SYNOPKG_PKG_STATUS}" == "INSTALL" ]; then
        # Clone the repository
<<<<<<< HEAD
         ${GIT} clone -q -b ${wizard_fork_branch:=master} ${wizard_fork_url:=git://github.com/CouchPotato/CouchPotatoServer.git} ${SYNOPKG_PKGVAR}/CouchPotatoServer >> ${INST_LOG} 2>&1
=======
        ${GIT} clone -q -b ${wizard_fork_branch:=master} ${wizard_fork_url:=git://github.com/CouchPotato/CouchPotatoServer.git} ${SYNOPKG_PKGDEST}/var/CouchPotatoServer
>>>>>>> d44affc1a5e8ba9a78392acb39f2e45161c48f9b
    fi

    # Create logs directory, otherwise it might not start
    mkdir -p "$(dirname ${LOG_FILE})"

    # If nessecary, add user also to the old group
    syno_user_add_to_legacy_group "${EFF_USER}" "${USER}" "${LEGACY_GROUP}"

    # Remove legacy user
    # Commands of busybox from spk/python
    delgroup "${USER}" "users"
    deluser "${USER}"
}

