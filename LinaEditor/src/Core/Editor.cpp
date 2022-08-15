/*
This file is a part of: Lina Engine
https://github.com/inanevin/LinaEngine

Author: Inan Evin
http://www.inanevin.com

Copyright (c) [2018-] [Inan Evin]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Core/Editor.hpp"
#include "Settings/EngineSettings.hpp"
#include "Settings/EditorSettings.hpp"
#include "Settings/RenderSettings.hpp"
#include "Core/ResourceDataManager.hpp"

namespace Lina::Editor
{
    void EditorManager::VerifyStaticResources()
    {
        // Make sure the static resources needed are initialized.
        if (!Utility::FileExists("Resources/lina.enginesettings"))
        {
            EngineSettings s;
            Resources::SaveArchiveToFile<EngineSettings>("Resources/lina.enginesettings", s);
        }
        if (!Utility::FileExists("Resources/lina.rendersettings"))
        {
            RenderSettings s;
            Resources::SaveArchiveToFile<RenderSettings>("Resources/lina.rendersettings", s);
        }
        if (!Utility::FileExists("Resources/lina.resourcedata"))
        {
            Resources::ResourceDataManager s;
            Resources::SaveArchiveToFile<Resources::ResourceDataManager>("Resources/lina.resourcedata", s);
        }
    }
} // namespace Lina::Editor
