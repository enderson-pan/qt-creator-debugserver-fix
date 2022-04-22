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

#pragma once

#include "clangdiagnosticconfigsmodel.h"

#include <utils/fileutils.h>

#include <QObject>
#include <QStringList>
#include <QVersionNumber>

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

namespace ProjectExplorer { class Project; }

namespace CppEditor {

class CPPEDITOR_EXPORT CppCodeModelSettings : public QObject
{
    Q_OBJECT

public:
    enum PCHUsage {
        PchUse_None = 1,
        PchUse_BuildSystem = 2
    };

public:
    void fromSettings(QSettings *s);
    void toSettings(QSettings *s);

public:
    Utils::Id clangDiagnosticConfigId() const;
    void setClangDiagnosticConfigId(const Utils::Id &configId);
    static Utils::Id defaultClangDiagnosticConfigId() ;
    const ClangDiagnosticConfig clangDiagnosticConfig() const;

    ClangDiagnosticConfigs clangCustomDiagnosticConfigs() const;
    void setClangCustomDiagnosticConfigs(const ClangDiagnosticConfigs &configs);

    bool enableLowerClazyLevels() const;
    void setEnableLowerClazyLevels(bool yesno);

    PCHUsage pchUsage() const;
    void setPCHUsage(PCHUsage pchUsage);

    bool interpretAmbigiousHeadersAsCHeaders() const;
    void setInterpretAmbigiousHeadersAsCHeaders(bool yesno);

    bool skipIndexingBigFiles() const;
    void setSkipIndexingBigFiles(bool yesno);

    int indexerFileSizeLimitInMb() const;
    void setIndexerFileSizeLimitInMb(int sizeInMB);

    void setCategorizeFindReferences(bool categorize) { m_categorizeFindReferences = categorize; }
    bool categorizeFindReferences() const { return m_categorizeFindReferences; }

signals:
    void clangDiagnosticConfigsInvalidated(const QVector<Utils::Id> &configId);
    void changed();

private:
    PCHUsage m_pchUsage = PchUse_BuildSystem;
    bool m_interpretAmbigiousHeadersAsCHeaders = false;
    bool m_skipIndexingBigFiles = true;
    int m_indexerFileSizeLimitInMB = 5;
    ClangDiagnosticConfigs m_clangCustomDiagnosticConfigs;
    Utils::Id m_clangDiagnosticConfigId;
    bool m_enableLowerClazyLevels = true; // For UI behavior only
    bool m_categorizeFindReferences = false; // Ephemeral!
};

class CPPEDITOR_EXPORT ClangdSettings : public QObject
{
    Q_OBJECT
public:
    class CPPEDITOR_EXPORT Data
    {
    public:
        QVariantMap toMap() const;
        void fromMap(const QVariantMap &map);

        friend bool operator==(const Data &s1, const Data &s2)
        {
            return s1.useClangd == s2.useClangd
                    && s1.executableFilePath == s2.executableFilePath
                    && s1.sessionsWithOneClangd == s2.sessionsWithOneClangd
                    && s1.workerThreadLimit == s2.workerThreadLimit
                    && s1.enableIndexing == s2.enableIndexing
                    && s1.autoIncludeHeaders == s2.autoIncludeHeaders
                    && s1.documentUpdateThreshold == s2.documentUpdateThreshold;
        }
        friend bool operator!=(const Data &s1, const Data &s2) { return !(s1 == s2); }

        Utils::FilePath executableFilePath;
        QStringList sessionsWithOneClangd;
        int workerThreadLimit = 0;
        bool useClangd = true;
        bool enableIndexing = true;
        bool autoIncludeHeaders = false;
        int documentUpdateThreshold = 500;
    };

    ClangdSettings(const Data &data) : m_data(data) {}

    static ClangdSettings &instance();
    bool useClangd() const;

    static void setDefaultClangdPath(const Utils::FilePath &filePath);
    Utils::FilePath clangdFilePath() const;
    bool indexingEnabled() const { return m_data.enableIndexing; }
    bool autoIncludeHeaders() const { return m_data.autoIncludeHeaders; }
    int workerThreadLimit() const { return m_data.workerThreadLimit; }
    int documentUpdateThreshold() const { return m_data.documentUpdateThreshold; }

    enum class Granularity { Project, Session };
    Granularity granularity() const;

    void setData(const Data &data);
    Data data() const { return m_data; }

    static QVersionNumber clangdVersion(const Utils::FilePath &clangdFilePath);
    QVersionNumber clangdVersion() const { return clangdVersion(clangdFilePath()); }

#ifdef WITH_TESTS
    static void setUseClangd(bool use);
    static void setClangdFilePath(const Utils::FilePath &filePath);
#endif

signals:
    void changed();

private:
    ClangdSettings();

    void loadSettings();
    void saveSettings();

    Data m_data;
};

class CPPEDITOR_EXPORT ClangdProjectSettings
{
public:
    ClangdProjectSettings(ProjectExplorer::Project *project);

    ClangdSettings::Data settings() const;
    void setSettings(const ClangdSettings::Data &data);
    bool useGlobalSettings() const { return m_useGlobalSettings; }
    void setUseGlobalSettings(bool useGlobal);

private:
    void loadSettings();
    void saveSettings();

    ProjectExplorer::Project * const m_project;
    ClangdSettings::Data m_customSettings;
    bool m_useGlobalSettings = true;
};

} // namespace CppEditor
