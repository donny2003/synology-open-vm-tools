# Package specific behaviors
# Sourced script by generic installer and start-stop-status scripts

# Add ffmpeg and ifself to path
PYTHON_DIR="/var/packages/python38/target"
PYTHONENV="${SYNOPKG_PKGDEST}/env"
VIRTUALENV="${PYTHON_DIR}/bin/virtualenv"
WHEELHOUSE=${SYNOPKG_PKGDEST}/share/wheelhouse
FFMPEG_DIR="/var/packages/ffmpeg/target"
PATH="${SYNOPKG_PKGDEST}/env/bin:${SYNOPKG_PKGDEST}/bin:${FFMPEG_DIR}/bin:${PYTHON_DIR}/bin:${PATH}"

# Service configuration. Change http and htsp ports here and in conf/tvheadend.sc for non-standard ports
HTTP=9981
HTSP=9982

# Replace generic service startup, run service in background
<<<<<<< HEAD
GRPN=`id -gn ${EFF_USER}`
HOME_DIR="${SYNOPKG_PKGVAR}"
DVR_LOG_DIR="${SYNOPKG_PKGVAR}/dvr/log"
SAVE_DIR="/tmp/tvheadend-recording-backup"
SERVICE_COMMAND="${SYNOPKG_PKGDEST}/bin/tvheadend -f -u ${EFF_USER} -g ${GRPN} --http_port ${HTTPP} --htsp_port ${HTSPP} -c ${HOME_DIR} -p ${PID_FILE}"
=======
GRPN=$(id -gn ${EFF_USER})
UPGRADE_CFG_DIR="${SYNOPKG_PKGVAR}/dvr/config"
SERVICE_COMMAND="${SYNOPKG_PKGDEST}/bin/tvheadend -f -u ${EFF_USER} -g ${GRPN} --http_port ${HTTP} --htsp_port ${HTSP} -c ${SYNOPKG_PKGVAR} -p ${PID_FILE}"
>>>>>>> d44affc1a5e8ba9a78392acb39f2e45161c48f9b
SVC_BACKGROUND=yes

# Group configuration to manage permissions of recording folders
GROUP=sc-media

service_postinst ()
{
    # Encrypt password
    wizard_password=$(echo -n "TVHeadend-Hide-${wizard_password:=admin}" | openssl enc -a)

    # Edit the password configuration according to the wizard
    sed -i -e "s/@password@/${wizard_password}/g" ${SYNOPKG_PKGVAR}/passwd/a927e30a755504f9784f23a4efac5109

<<<<<<< HEAD
    # Fix fontconfig links
    CONFD_DIR="${SYNOPKG_PKGDEST}/etc/fonts/conf.d"
    FONTS_DIR="${SYNOPKG_PKGDEST}/share/fontconfig/conf.avail"
    echo "Fixing fontconfig links" >> ${INST_LOG}
    for FONT_FILE in ${CONFD_DIR}/*.conf
    do
        FONT_NAME=`basename "${FONT_FILE}"`
        $LN "${FONTS_DIR}/${FONT_NAME}" "${CONFD_DIR}/${FONT_NAME}" >> ${INST_LOG}
    done
}

service_preupgrade ()
{
    # For backwards compatibility on DSM6
    if [ $SYNOPKG_DSM_VERSION_MAJOR -gt 5 ]; then
        # Backup potential recordings from package root directory
        echo "Save potential recordings from package root directory..." >> ${INST_LOG}
        for logfile in ${DVR_LOG_DIR}/*
        do
            REC_FILE=`grep -e 'filename' ${logfile} | awk -F'"' '{print $4}'`
            # Check whether recording actually exists, otherwise ignore the entry
            if [ -e "${REC_FILE}" ]; then
                REC_DIR=`dirname "${REC_FILE}"`
                REAL_DIR=`realpath "${REC_DIR}"`
                TRUNC_DIR=`echo "${REAL_DIR}" | cut -c9-28`
                CHECK_VAR=`echo "${REAL_DIR}" | cut -c29-32`
                if [ ! "${REC_FILE}" = "" ] && [ "${TRUNC_DIR}" = "/@appstore/tvheadend" ] && [ ! "${CHECK_VAR}" = "/var" ]; then
                    REC_NAME=`basename "${REC_FILE}"`
                    REST_DIR=`echo "${REAL_DIR}" | cut -c30-`
                    DEST_FILE="${SAVE_DIR}/${REST_DIR}/${REC_NAME}"
                    echo "Saving: ${REC_FILE}" >> ${INST_LOG}
                    $MKDIR "${SAVE_DIR}/${REST_DIR}"
                    $CP "${REC_FILE}" "${DEST_FILE}"
                    # Replace any remaining target links in log files first
                    sed -i -e "s,/var/packages/tvheadend/target/,/usr/local/tvheadend/,g" "${logfile}"
                    # Adapt recording paths in dvr log files for restored files (postupgrade)
                    sed -i -e "s,/tvheadend/,/tvheadend/var/,g" "${logfile}"
                fi
            fi
        done
        # Move recording directories from package root into var
        UPGRADE_CFG_DIR="${SYNOPKG_PKGVAR}/dvr/config"
        echo "Move any recording directories from package root directory into var..." >> ${INST_LOG}
        for file in ${UPGRADE_CFG_DIR}/*
        do
            DVR_DIR=`grep -e 'storage\":' ${file} | awk -F'"' '{print $4}'`
            REAL_DIR=`realpath "${DVR_DIR}"`
            TRUNC_DIR=`echo "${REAL_DIR}" | cut -c9-28`
            CHECK_VAR=`echo "${REAL_DIR}" | cut -c29-32`
            # Replace any remaining target links in log files first
            sed -i -e "s,/var/packages/tvheadend/target,/usr/local/tvheadend,g" "${file}"
            if [ "${TRUNC_DIR}" = "/@appstore/tvheadend" ] && [ ! "${CHECK_VAR}" = "/var" ]; then
                # Move recording paths in recording profiles into var
                sed -i -e "s,/tvheadend,/tvheadend/var,g" "${file}"
            fi
        done
    fi
=======
    # EPG Grabber (zap2epg) - Create a Python virtualenv
    ${VIRTUALENV} --system-site-packages ${PYTHONENV}

    # EPG Grabber (zap2epg) - Install the wheels/requirements
    ${SYNOPKG_PKGDEST}/env/bin/pip install \
             --no-deps --no-index --no-input --upgrade \
             --force-reinstall --find-links \
             ${WHEELHOUSE} ${WHEELHOUSE}/*.whl
>>>>>>> d44affc1a5e8ba9a78392acb39f2e45161c48f9b
}

service_postupgrade ()
{
    # Need to enforce correct permissions for recording directories on upgrades
<<<<<<< HEAD
    echo "Adding ${GROUP} group permissions on recording directories:"  >> ${INST_LOG}
    UPGRADE_CFG_DIR="${SYNOPKG_PKGVAR}/dvr/config"
=======
    echo "Adding ${GROUP} group permissions on recording directories:"
>>>>>>> d44affc1a5e8ba9a78392acb39f2e45161c48f9b
    for file in ${UPGRADE_CFG_DIR}/*
    do
        DVR_DIR=$(grep -e 'storage\":' ${file} | awk -F'"' '{print $4}')
        # Exclude directories in @appstore as ACL permissions srew up package installations
        TRUNC_DIR=$(echo "$(realpath ${DVR_DIR})" | awk -F/ '{print "/"$3}')
        if [ "${TRUNC_DIR}" = "/@appstore" ]; then
<<<<<<< HEAD
            echo "Skip: ${DVR_DIR} (system directory)" >> ${INST_LOG}
        elif [ $SYNOPKG_DSM_VERSION_MAJOR -lt 7 ]; then
            echo "Done: ${DVR_DIR}" >> ${INST_LOG}
=======
            echo "Skip: ${DVR_DIR} (system directory)"
        elif [ $SYNOPKG_DSM_VERSION_MAJOR -lt 7 ]; then
            echo "Done: ${DVR_DIR}"
>>>>>>> d44affc1a5e8ba9a78392acb39f2e45161c48f9b
            set_syno_permissions "${DVR_DIR}" "${GROUP}"
        fi
    done

    # For backwards compatibility, restore ownership of package system directories
<<<<<<< HEAD
    echo "Restore '${EFF_USER}' unix permissions on package system directories" >> ${INST_LOG}
    if [ $SYNOPKG_DSM_VERSION_MAJOR -lt 6 ]; then
        synoacltool -del "${SYNOPKG_PKGDEST}" >> ${INST_LOG} 2>&1
        chown ${EFF_USER}:root "/var/packages/tvheadend" >> ${INST_LOG} 2>&1
    elif [ $SYNOPKG_DSM_VERSION_MAJOR == 6 ]; then
        chown ${EFF_USER}:${USER} "/var/packages/tvheadend/target" >> ${INST_LOG} 2>&1
=======
    if [ $SYNOPKG_DSM_VERSION_MAJOR == 6 ]; then
        echo "Restore '${EFF_USER}' unix permissions on package system directories"
        chown ${EFF_USER}:${USER} "${SYNOPKG_PKGDEST}"
>>>>>>> d44affc1a5e8ba9a78392acb39f2e45161c48f9b
        set_unix_permissions "${SYNOPKG_PKGVAR}"
    fi
}
