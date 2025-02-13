/*  Copyright 2008 Jozsef Bekes

    This file is part of gtalX.

    gtalX is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    gtalX is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with gtalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <map>
#include <vector>

#include <qstring.h>

/**
	@author bjdodo <erjoe@JUNIOR>
*/
class CSettingsManager
{
public:
    CSettingsManager(const QString& settingsFileName);
    ~CSettingsManager();

    //std::map<QString, QString>& getSettings();

    bool saveSettings();

    const QString operator[] (const QString& key);
    void SetSetting(const QString& key, const QString& val);

protected:
    std::map<QString, QString> m_settings;
    std::vector<QString> m_unorderedSettings;
    QString m_FileName;
};

#endif
