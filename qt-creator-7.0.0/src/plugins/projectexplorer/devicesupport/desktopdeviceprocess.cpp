/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#include "desktopdeviceprocess.h"

#include "idevice.h"
#include "../runcontrol.h"

#include <utils/environment.h>
#include <utils/qtcassert.h>
#include <utils/qtcprocess.h>

using namespace Utils;

namespace ProjectExplorer {
namespace Internal {

DesktopDeviceProcess::DesktopDeviceProcess(const QSharedPointer<const IDevice> &device,
                                           QObject *parent)
    : DeviceProcess(device, ProcessMode::Writer, parent)
{
}

void DesktopDeviceProcess::start(const Runnable &runnable)
{
    QTC_ASSERT(state() == QProcess::NotRunning, return);
    if (runnable.environment.size())
        setEnvironment(runnable.environment);
    setWorkingDirectory(runnable.workingDirectory);
    setCommand(runnable.command);
    QtcProcess::start();
}

void DesktopDeviceProcess::interrupt()
{
    device()->signalOperation()->interruptProcess(processId());
}

} // namespace Internal
} // namespace ProjectExplorer
