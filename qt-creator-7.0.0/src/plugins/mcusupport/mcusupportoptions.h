/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
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

#pragma once

#include <utils/environmentfwd.h>
#include "mcusupport_global.h"
#include "mcukitmanager.h"

#include <QObject>
#include <QVector>
#include <QVersionNumber>

QT_FORWARD_DECLARE_CLASS(QWidget)

namespace Utils {
class FilePath;
class PathChooser;
class InfoLabel;
} // namespace Utils

namespace ProjectExplorer {
class Kit;
class ToolChain;
} // namespace ProjectExplorer

namespace McuSupport {
namespace Internal {

class McuAbstractPackage;
class McuToolChainPackage;
class McuTarget;

class McuSdkRepository
{
public:
    QVector<McuAbstractPackage *> packages;
    QVector<McuTarget *> mcuTargets;

    void deletePackagesAndTargets();
};

class McuSupportOptions : public QObject
{
    Q_OBJECT

public:
    explicit McuSupportOptions(QObject *parent = nullptr);
    ~McuSupportOptions() override;

    McuAbstractPackage *qtForMCUsSdkPackage = nullptr;
    McuSdkRepository sdkRepository;

    void setQulDir(const Utils::FilePath &dir);
    static void setKitEnvironment(ProjectExplorer::Kit *,
                                  const McuTarget *,
                                  const McuAbstractPackage *);
    static void updateKitEnvironment(ProjectExplorer::Kit *, const McuTarget *);
    static void remapQul2xCmakeVars(ProjectExplorer::Kit *, const Utils::EnvironmentItems &);
    static Utils::FilePath qulDirFromSettings();
    static McuKitManager::UpgradeOption askForKitUpgrades();

    static void registerQchFiles();
    static void registerExamples();

    static const QVersionNumber &minimalQulVersion();

    void checkUpgradeableKits();
    void populatePackagesAndTargets();

    static bool kitsNeedQtVersion();

    bool automaticKitCreationEnabled() const;
    void setAutomaticKitCreationEnabled(const bool enabled);
    void writeGeneralSettings() const;
    static bool automaticKitCreationFromSettings();
private:
    void deletePackagesAndTargets();

    bool m_automaticKitCreation = true;
signals:
    void packagesChanged();
};


} // namespace Internal
} // namespace McuSupport
