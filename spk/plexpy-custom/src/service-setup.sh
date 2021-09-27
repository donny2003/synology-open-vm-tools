PYTHON_DIR="/var/packages/python3/target/bin"
GIT_DIR="/var/packages/git/target/bin"
PATH="${SYNOPKG_PKGDEST}/bin:${SYNOPKG_PKGDEST}/env/bin:${PYTHON_DIR}:${GIT_DIR}:${PATH}"
PYTHON="${SYNOPKG_PKGDEST}/env/bin/python"
GIT="${GIT_DIR}/git"
<<<<<<< HEAD
VIRTUALENV="${PYTHON_DIR}/virtualenv"
PLEXPY="${SYNOPKG_PKGVAR}/plexpy/PlexPy.py"
CFG_FILE="${SYNOPKG_PKGVAR}/config.ini"
=======
VIRTUALENV="${PYTHON_DIR}/python3 -m venv"
PLEXPY="${SYNOPKG_PKGDEST}/var/plexpy/PlexPy.py"
CFG_FILE="${SYNOPKG_PKGDEST}/var/config.ini"
>>>>>>> d44affc1a5e8ba9a78392acb39f2e45161c48f9b

SERVICE_COMMAND="${PYTHON} ${PLEXPY} --daemon --pidfile ${PID_FILE} --config ${CFG_FILE} --datadir ${SYNOPKG_PKGVAR}/"

GROUP="sc-download"
LEGACY_GROUP="sc-media"


validate_preinst ()
{
    # Check fork
    if [ "${SYNOPKG_PKG_STATUS}" == "INSTALL" ] && ! ${GIT} ls-remote --heads --exit-code ${wizard_fork_url:=git://github.com/Tautulli/Tautulli.git} ${wizard_fork_branch:=master} > /dev/null 2>&1; then
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
        ${GIT} clone -q -b ${wizard_fork_branch:=master} ${wizard_fork_url:=git://github.com/Tautulli/Tautulli.git} ${SYNOPKG_PKGVAR}/plexpy >> ${INST_LOG} 2>&1
=======
        ${GIT} clone -q -b ${wizard_fork_branch:=master} ${wizard_fork_url:=git://github.com/Tautulli/Tautulli.git} ${SYNOPKG_PKGDEST}/var/plexpy
>>>>>>> d44affc1a5e8ba9a78392acb39f2e45161c48f9b
    fi

    # Remove legacy user
    # Commands of busybox from spk/python
    delgroup "${USER}" "users"
    deluser "${USER}"
}

