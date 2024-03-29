VIRTUALENV="/var/packages/python/target/bin/virtualenv"
PATH="${SYNOPKG_PKGDEST}/bin:${PATH}"

GROUP="sc-download"
<<<<<<< HEAD
INST_LOG="${SYNOPKG_PKGVAR}/${SYNOPKG_PKGNAME}_installer.log"
CFG_FILE="${SYNOPKG_PKGVAR}/pyload.conf"
=======
CFG_FILE="${SYNOPKG_PKGDEST}/var/pyload.conf"
>>>>>>> d44affc1a5e8ba9a78392acb39f2e45161c48f9b
PYLOAD="${SYNOPKG_PKGDEST}/env/bin/python ${SYNOPKG_PKGDEST}/share/pyload/pyLoadCore.py"
DOWNLOAD_DIR="${wizard_download_dir:=/volume1/downloads}"

validate_preinst ()
{
    if [ "${SYNOPKG_PKG_STATUS}" == "INSTALL" ]; then
        # Check directory
        if [ ! -d ${DOWNLOAD_DIR} ]; then
            echo "Download directory ${DOWNLOAD_DIR} does not exist."
            exit 1
        fi
    fi
    exit 0
}

# changes 3-character Synology language code to ISO 639-1 (2-charater) code.
lng2iso()
{
    case $1 in
        enu|fre|ita|nld|plk|rus|sve|trk)
            echo "${1%?}"
            ;;
        ger)
            echo "de"
            ;;            
        spn)
            echo "es"
            ;;
        ptb|ptg)
            echo "pt_BR"
            ;;
        *)
            echo "en"
            ;;
    esac
}

service_postinst ()
{
    # Create a Python virtualenv
    ${VIRTUALENV} --system-site-packages ${SYNOPKG_PKGDEST}/env

    # Adjust ownership
    chown -R ${EFF_USER}:${GROUP} ${SYNOPKG_PKGDEST}

    # Log installation information
    ${PYLOAD} --version

    echo "System check:"
    # result of system check (only DSM>=6 have grep with perl capabilities -P)
    if [ "$(grep --version 2> /dev/null | grep -o "GNU grep")" == "GNU grep" ]; then
        echo -e "\n\n" | ${PYLOAD} --setup 2> /dev/null | grep -Pzo "##(.|\n)*^You can abort.*$" | grep -v "^$\|hit enter\|^You can abort"
    else
        echo -e "\n\n" | ${PYLOAD} --setup 2> /dev/null | tail -n +12 | grep -v "^$\|hit enter\|^You can abort\|^Continue with setup"
    fi

    if [ "${SYNOPKG_PKG_STATUS}" == "INSTALL" ]; then

        # Edit the configuration according to the wizard
        sed -i -e "s|@W_DOWNLOAD_DIR@|${DOWNLOAD_DIR}|" "${CFG_FILE}"
        # Set language to the DSM language
        sed -i -e "s|@LNG@|$(lng2iso ${SYNOPKG_DSM_LANGUAGE})|" "${CFG_FILE}"
        # Set user and group for permissions
        sed -i -e "s|@GROUP_NAME@|${GROUP}|" "${CFG_FILE}"
        sed -i -e "s|@USER_NAME@|${EFF_USER}|" "${CFG_FILE}"

        # hash password
        SALT=$((RANDOM%99999+10000))
        SALTED_PW_HASH=${SALT}$(echo -n "${SALT}${wizard_password}" | openssl dgst -sha1 2>/dev/null | cut -d" " -f2)

        # init DB & add 'admin' user
<<<<<<< HEAD
        echo -n "4" > "${SYNOPKG_PKGVAR}/files.version"
        sqlite3 "${SYNOPKG_PKGVAR}/files.db" < "${SYNOPKG_PKGVAR}/pyload_init.sql" || exit 1  >> ${INST_LOG}  2>&1
        sqlite3 "${SYNOPKG_PKGVAR}/files.db" "INSERT INTO users (name, password) VALUES ('admin', '${SALTED_PW_HASH}')" || exit 1  >> ${INST_LOG}  2>&1

        # Adjust ownership of installed files
        chown -R ${EFF_USER}:${GROUP} ${SYNOPKG_PKGVAR}  >> ${INST_LOG}  2>&1
=======
        echo -n "4" > "${SYNOPKG_PKGDEST}/var/files.version"
        sqlite3 "${SYNOPKG_PKGDEST}/var/files.db" < "${SYNOPKG_PKGDEST}/var/pyload_init.sql" || exit 1
        sqlite3 "${SYNOPKG_PKGDEST}/var/files.db" "INSERT INTO users (name, password) VALUES ('admin', '${SALTED_PW_HASH}')" || exit 1

        # Adjust ownership of installed files
        chown -R ${EFF_USER}:${GROUP} ${SYNOPKG_PKGDEST}/var
>>>>>>> d44affc1a5e8ba9a78392acb39f2e45161c48f9b
    fi

    exit 0
}

