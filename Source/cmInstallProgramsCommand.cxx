/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile$
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#include "cmInstallProgramsCommand.h"
#include "cmCacheManager.h"

// cmExecutableCommand
bool cmInstallProgramsCommand::InitialPass(std::vector<std::string>& args)
{
  if(args.size() < 2)
    {
    this->SetError("called with incorrect number of arguments");
    return false;
    }

  cmTargets &tgts = m_Makefile->GetTargets();
  std::vector<std::string>::iterator s = args.begin();
  if (tgts.find("INSTALL_PROGRAMS") != tgts.end())
    {
    tgts["INSTALL_PROGRAMS"].SetInstallPath(args[0].c_str());
    }
  ++s;
  for (;s != args.end(); ++s)
    {
    m_FinalArgs.push_back(*s);
    }
  
  return true;
}

void cmInstallProgramsCommand::FinalPass() 
{
  cmTargets &tgts = m_Makefile->GetTargets();

  if (tgts.find("INSTALL_PROGRAMS") == tgts.end())
    {
    return;
    }
  
  // two different options
  if (m_FinalArgs.size() > 1)
    {
    // for each argument, get the programs 
    for (std::vector<std::string>::iterator s = m_FinalArgs.begin();
         s != m_FinalArgs.end(); ++s)
      {
      // replace any variables
      std::string temps = *s;
      m_Makefile->ExpandVariablesInString(temps);
      // add to the result
      tgts["INSTALL_PROGRAMS"].GetSourceLists().push_back(temps);
      }
    }
  else     // reg exp list
    {
    std::vector<std::string> programs;
    cmSystemTools::Glob(m_Makefile->GetCurrentDirectory(),
                        m_FinalArgs[0].c_str(), programs);
    
    std::vector<std::string>::iterator s = programs.begin();
    // for each argument, get the programs 
    for (;s != programs.end(); ++s)
      {
      tgts["INSTALL_PROGRAMS"].GetSourceLists().push_back(*s);
      }
    }
}

      
