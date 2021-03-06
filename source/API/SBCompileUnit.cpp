//===-- SBCompileUnit.cpp ---------------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "lldb/API/SBCompileUnit.h"
#include "lldb/API/SBLineEntry.h"
#include "lldb/API/SBStream.h"
#include "lldb/Symbol/CompileUnit.h"
#include "lldb/Symbol/LineEntry.h"
#include "lldb/Symbol/LineTable.h"
#include "lldb/Core/Log.h"

using namespace lldb;
using namespace lldb_private;


SBCompileUnit::SBCompileUnit () :
    m_opaque_ptr (NULL)
{
}

SBCompileUnit::SBCompileUnit (lldb_private::CompileUnit *lldb_object_ptr) :
    m_opaque_ptr (lldb_object_ptr)
{
}

SBCompileUnit::SBCompileUnit(const SBCompileUnit &rhs) :
    m_opaque_ptr (rhs.m_opaque_ptr)
{
}

const SBCompileUnit &
SBCompileUnit::operator = (const SBCompileUnit &rhs)
{
    m_opaque_ptr = rhs.m_opaque_ptr;
    return *this;
}


SBCompileUnit::~SBCompileUnit ()
{
    m_opaque_ptr = NULL;
}

SBFileSpec
SBCompileUnit::GetFileSpec () const
{
    SBFileSpec file_spec;
    if (m_opaque_ptr)
        file_spec.SetFileSpec(*m_opaque_ptr);
    return file_spec;
}

uint32_t
SBCompileUnit::GetNumLineEntries () const
{
    if (m_opaque_ptr)
    {
        LineTable *line_table = m_opaque_ptr->GetLineTable ();
        if (line_table)
            return line_table->GetSize();
    }
    return 0;
}

SBLineEntry
SBCompileUnit::GetLineEntryAtIndex (uint32_t idx) const
{
    Log *log(lldb_private::GetLogIfAllCategoriesSet (LIBLLDB_LOG_API));

    SBLineEntry sb_line_entry;
    if (m_opaque_ptr)
    {
        LineTable *line_table = m_opaque_ptr->GetLineTable ();
        if (line_table)
        {
            LineEntry line_entry;
            if (line_table->GetLineEntryAtIndex(idx, line_entry))
                sb_line_entry.SetLineEntry(line_entry);
        }
    }
    
    if (log)
    {
        SBStream sstr;
        sb_line_entry.GetDescription (sstr);
        log->Printf ("SBCompileUnit(%p)::GetLineEntryAtIndex (idx=%u) => SBLineEntry(%p): '%s'", 
                     m_opaque_ptr, idx, sb_line_entry.get(), sstr.GetData());
    }

    return sb_line_entry;
}

uint32_t
SBCompileUnit::FindLineEntryIndex (uint32_t start_idx, uint32_t line, SBFileSpec *inline_file_spec) const
{
    const bool exact = true;
    return FindLineEntryIndex (start_idx, line, inline_file_spec, exact);
}

uint32_t
SBCompileUnit::FindLineEntryIndex (uint32_t start_idx, uint32_t line, SBFileSpec *inline_file_spec, bool exact) const
{
    Log *log(lldb_private::GetLogIfAllCategoriesSet (LIBLLDB_LOG_API));

    uint32_t index = UINT32_MAX;
    if (m_opaque_ptr)
    {
        FileSpec file_spec;
        if (inline_file_spec && inline_file_spec->IsValid())
            file_spec = inline_file_spec->ref();
        else
            file_spec = *m_opaque_ptr;

        
        index = m_opaque_ptr->FindLineEntry (start_idx,
                                             line,
                                             inline_file_spec ? inline_file_spec->get() : NULL,
                                             exact,
                                             NULL);
    }

    if (log)
    {
        SBStream sstr;
        if (index == UINT32_MAX)
        {
            log->Printf ("SBCompileUnit(%p)::FindLineEntryIndex (start_idx=%u, line=%u, SBFileSpec(%p)) => NOT FOUND", 
                         m_opaque_ptr, start_idx, line, inline_file_spec ? inline_file_spec->get() : NULL);
        }
        else
        {
            log->Printf ("SBCompileUnit(%p)::FindLineEntryIndex (start_idx=%u, line=%u, SBFileSpec(%p)) => %u", 
                         m_opaque_ptr, start_idx, line, inline_file_spec ? inline_file_spec->get() : NULL, index);
        }
    }

    return index;
}

uint32_t
SBCompileUnit::GetNumSupportFiles () const
{
    if (m_opaque_ptr)
    {
	FileSpecList& support_files = m_opaque_ptr->GetSupportFiles ();
	return support_files.GetSize();
    }
    return 0;
}

SBFileSpec
SBCompileUnit::GetSupportFileAtIndex (uint32_t idx) const
{
    Log *log(lldb_private::GetLogIfAllCategoriesSet (LIBLLDB_LOG_API));

    SBFileSpec sb_file_spec;
    if (m_opaque_ptr)
    {
	FileSpecList &support_files = m_opaque_ptr->GetSupportFiles ();
	FileSpec file_spec = support_files.GetFileSpecAtIndex(idx);
	sb_file_spec.SetFileSpec(file_spec);
    }
    
    if (log)
    {
        SBStream sstr;
        sb_file_spec.GetDescription (sstr);
        log->Printf ("SBCompileUnit(%p)::GetGetFileSpecAtIndex (idx=%u) => SBFileSpec(%p): '%s'", 
                     m_opaque_ptr, idx, sb_file_spec.get(), sstr.GetData());
    }

    return sb_file_spec;
}

uint32_t
SBCompileUnit::FindSupportFileIndex (uint32_t start_idx, const SBFileSpec &sb_file, bool full)
{
    if (m_opaque_ptr)
    {
	FileSpecList &support_files = m_opaque_ptr->GetSupportFiles ();
	return support_files.FindFileIndex(start_idx, sb_file.ref(), full);
    }
    return 0;
}

bool
SBCompileUnit::IsValid () const
{
    return m_opaque_ptr != NULL;
}

bool
SBCompileUnit::operator == (const SBCompileUnit &rhs) const
{
    return m_opaque_ptr == rhs.m_opaque_ptr;
}

bool
SBCompileUnit::operator != (const SBCompileUnit &rhs) const
{
    return m_opaque_ptr != rhs.m_opaque_ptr;
}

const lldb_private::CompileUnit *
SBCompileUnit::operator->() const
{
    return m_opaque_ptr;
}

const lldb_private::CompileUnit &
SBCompileUnit::operator*() const
{
    return *m_opaque_ptr;
}

lldb_private::CompileUnit *
SBCompileUnit::get ()
{
    return m_opaque_ptr;
}

void
SBCompileUnit::reset (lldb_private::CompileUnit *lldb_object_ptr)
{
    m_opaque_ptr = lldb_object_ptr;
}

    
bool
SBCompileUnit::GetDescription (SBStream &description)
{
    Stream &strm = description.ref();

    if (m_opaque_ptr)
    {
        m_opaque_ptr->Dump (&strm, false);
    }
    else
        strm.PutCString ("No value");
    
    return true;
}
