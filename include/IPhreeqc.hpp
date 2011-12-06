/*! @file IPhreeqc.hpp
	@brief C++ Documentation
*/

#ifndef INC_IPHREEQC_HPP
#define INC_IPHREEQC_HPP

#include <exception>
#include <list>
#include <vector>
#include <cstdarg>
#include "IPhreeqcCallbacks.h"      /* PFN_PRERUN_CALLBACK, PFN_POSTRUN_CALLBACK, PFN_CATCH_CALLBACK */
#include "Var.h"                    /* VRESULT */
#include "PHRQ_io.h"

#if defined(_WINDLL)
#define IPQ_DLL_EXPORT __declspec(dllexport)
#else
#define IPQ_DLL_EXPORT
#endif

class Phreeqc;
class IErrorReporter;
class CSelectedOutput;

/**
 * @class IPhreeqcStop
 *
 * @brief This class is derived from std::exception and is thrown
 * when an unrecoverable error has occured.
 */
class IPQ_DLL_EXPORT IPhreeqcStop : std::exception
{
};

/**
 * @class IPhreeqc
 *
 * @brief Provides an interface to PHREEQC (Version 2)--A Computer
 * Program for Speciation, Batch-Reaction, One-Dimensional Transport,
 * and Inverse Geochemical Calculations
 */
class IPQ_DLL_EXPORT IPhreeqc : public PHRQ_io
{
public:
	/**
	 * Constructor.
     *  \anchor IPhreeqc_cpp
     *  @par Example:
     *  \include IPhreeqc.cpp
	 */
	IPhreeqc(void);

	/**
	 * Destructor
	 */
	virtual ~IPhreeqc(void);

public:

	/**
	 *  Accumlulate line(s) for input to phreeqc.
	 *  @param line             The line(s) to add for input to phreeqc.
	 *  @retval VR_OK           Success
	 *  @retval VR_OUTOFMEMORY  Out of memory
     *  @see                    ClearAccumulatedLines, OutputAccumulatedLines, RunAccumulated
	 */
	VRESULT                  AccumulateLine(const char *line);

	/**
	 *  Appends the given error message and increments the error count.
	 *  Internally used to create an error condition.
	 *  @param error_msg        The error message to display.
	 *  @returns                The current error count.
     *  @see                    GetErrorString, GetErrorStringLine, GetErrorStringLineCount, OutputErrorString
	 */
	size_t                   AddError(const char* error_msg);

	/**
	 *  Appends the given warning message and increments the warning count.
	 *  Internally used to create a warning condition.
	 *  @param warning_msg      The warning message to display.
	 *  @returns                The current warning count.
     *  @see                    GetWarningString, GetWarningStringLine, GetWarningStringLineCount, OutputWarningString
	 */
	size_t                   AddWarning(const char* warning_msg);

	/**
	 *  Clears the accumulated input buffer.  Input buffer is accumulated from calls to \ref AccumulateLine.
	 *  @see                    AccumulateLine, GetAccumulatedLines, OutputAccumulatedLines, RunAccumulated
	 */
	void                     ClearAccumulatedLines(void);

	/**
	 *  Retrieve the accumulated input string.  The accumulated input string can be run
	 *  with \ref RunAccumulated.
	 *  @returns                The accumulated input string.
	 *  @see                    AccumulateLine, ClearAccumulatedLines, OutputAccumulatedLines, RunAccumulated
	 */
	const std::string&       GetAccumulatedLines(void);

	/**
	 *  Retrieves the given component.
	 *  @param n                The zero-based index of the component to retrieve.
	 *  @return                 A null terminated string containing the given component.
	 *                          Returns an empty string if n is out of range.
	 *  @see                    GetComponentCount, ListComponents
	 */
	const char*              GetComponent(int n);

	/**
	 *  Retrieves the number of components in the current list of components.
	 *  @return                 The current count of components.
	 *  @see                    GetComponent, ListComponents
	 */
	size_t                   GetComponentCount(void);

	/**
	 *  Retrieves the current value of the dump file switch.
     *  @retval true            Output is written to the <B>DUMP</B> (<B><I>dump.out</I></B> if unspecified) file.
     *  @retval false           No output is written.
	 *  @see                    GetDumpStringLine, GetDumpStringLineCount, GetDumpStringOn, GetDumpString, SetDumpFileOn, SetDumpStringOn
	 */
	bool                     GetDumpFileOn(void)const;

	/**
	 *  Retrieves the string buffer containing <b>DUMP</b> output.
     *  @return                 A null terminated string containing <b>DUMP</b> output.
	 *  @pre
	 *      \ref SetDumpStringOn must have been set to true in order to recieve <b>DUMP</b> output.
	 *  @see                    GetDumpStringLine, GetDumpFileOn, GetDumpStringLineCount, GetDumpStringOn, SetDumpFileOn, SetDumpStringOn
	 */
	const char*              GetDumpString(void);

	/**
	 *  Retrieves the given dump line.
	 *  @param n                The zero-based index of the line to retrieve.
	 *  @return                 A null terminated string containing the given line.
	 *                          Returns an empty string if n is out of range.
     *  @pre                    \ref SetDumpStringOn must have been set to true.
	 *  @see                    GetDumpFileOn, GetDumpString, GetDumpStringLineCount, GetDumpStringOn, SetDumpFileOn, SetDumpStringOn
	 */
	const char*              GetDumpStringLine(int n);

	/**
	 *  Retrieves the number of lines in the current dump string buffer.
	 *  @return                 The number of lines.
     *  @pre                    \ref SetDumpStringOn must have been set to true.
	 *  @see                    GetDumpFileOn, GetDumpString, GetDumpStringLine, GetDumpStringOn, SetDumpFileOn, SetDumpStringOn
	 */
	int                      GetDumpStringLineCount(void)const;

	/**
	 *  Retrieves the current value of the dump string switch.
     *  @retval true            Output defined by the <B>DUMP</B> keyword is stored.
     *  @retval false           No output is stored.
	 *  @see                    GetDumpFileOn, GetDumpString, GetDumpStringLine, GetDumpStringLineCount, SetDumpFileOn, SetDumpStringOn
	 */
	bool                     GetDumpStringOn(void)const;

	/**
	 *  Retrieves the current value of the error file switch.
     *  @retval true            Errors are written to the <B><I>phreeqc.err</I></B> file.
     *  @retval false           No errors are written.
	 *  @see                    SetErrorFileOn
	 */
	bool                     GetErrorFileOn(void)const;

	/**
	 *  Retrieves the error messages from the last call to \ref RunAccumulated, \ref RunFile, \ref RunString, \ref LoadDatabase, or \ref LoadDatabaseString.
     *  @return                 A null terminated string containing error messages.
	 *  @see                    GetErrorStringLine, GetErrorStringLineCount, GetErrorFileOn, OutputErrorString, SetErrorFileOn
	 */
	const char*              GetErrorString(void);

	/**
	 *  Retrieves the given error line.
     *  @return                 A null terminated string containing the given line of the error string buffer.
	 *  @param n                The zero-based index of the line to retrieve.
	 *  @see                    GetErrorStringLineCount, OutputErrorString
	 */
	const char*              GetErrorStringLine(int n);

	/**
	 *  Retrieves the number of lines in the current error string buffer.
     *  @return                 The number of lines.
	 *  @see                    GetErrorStringLine, OutputErrorString
	 */
	int                      GetErrorStringLineCount(void)const;

	/**
	 *  Retrieves the current value of the log file switch.
     *  @retval true            Log messages are written to the <B><I>phreeqc.log</I></B> file.
     *  @retval false           No log messages are written.
     *  @remarks
     *      Logging must be enabled through the use of the KNOBS -logfile option in order to receive any log messages.
	 *  @see                    SetLogFileOn
	 */
	bool                     GetLogFileOn(void)const;

	/**
	 *  Retrieves the current value of the output file switch.
     *  @retval true            Output is written to the <B><I>phreeqc.out</I></B> file.
     *  @retval false           No output is written.
	 *  @see                    SetOutputFileOn
	 */
	bool                     GetOutputFileOn(void)const;

	/**
	 *  Retrieves the number of columns in the selected-output buffer.
     *  @return                 The number of columns.
	 *  @see                    GetSelectedOutputRowCount, GetSelectedOutputValue
	 */
	int                      GetSelectedOutputColumnCount(void)const;

	/**
	 *  Retrieves the selected-output file switch.
     *  @retval true            Output is written to the selected-output (<B><I>selected.out</I></B> if unspecified) file.
     *  @retval false           No output is written.
	 *  @see                    GetSelectedOutputValue, GetSelectedOutputColumnCount, GetSelectedOutputRowCount, SetSelectedOutputFileOn
	 */
	bool                     GetSelectedOutputFileOn(void)const;

	/**
	 *  Retrieves the number of rows in the selected-output buffer.
     *  @return                 The number of rows.
	 *  @see                    GetSelectedOutputColumnCount, GetSelectedOutputFileOn, GetSelectedOutputValue, SetSelectedOutputFileOn
	 */
	int                      GetSelectedOutputRowCount(void)const;

	/**
	 *  Returns the \c VAR associated with the specified row and column.
	 *  @param row              The row index.
	 *  @param col              The column index.
	 *  @param pVAR             Pointer to the \c VAR to recieve the requested data.
	 *  @retval VR_OK           Success.
	 *  @retval VR_INVALIDROW   The given row is out of range.
	 *  @retval VR_INVALIDCOL   The given column is out of range.
	 *  @retval VR_OUTOFMEMORY  Memory could not be allocated.
	 *  @retval VR_BADINSTANCE  The given id is invalid.
	 *  @see                    GetSelectedOutputColumnCount, GetSelectedOutputFileOn, GetSelectedOutputRowCount, SetSelectedOutputFileOn
	 *  @remarks
	 *      Row 0 contains the column headings to the selected_ouput.
	 *  @par Examples:
	 *  The headings will include a suffix and/or prefix in order to differentiate the
	 *  columns.
	 *  @htmlonly
	<p>
	<table border=1>

	<TR VALIGN="top">
	<TH width=65%>
	Input
	</TH>
	<TH width=35%>
	Headings
	</TH>
	</TR>

	<TR VALIGN="top">
	<TD width=65%>
	<CODE><PRE>
	  SELECTED_OUTPUT
		-reset false
		-totals Ca Na
	</PRE></CODE>
	</TD>
	<TD width=35%>
	<CODE><PRE>
	  Ca(mol/kgw)  Na(mol/kgw)
	</PRE></CODE>
	</TD>
	</TR>

	<TR VALIGN="top">
	<TD width=65%>
	<CODE><PRE>
	  SELECTED_OUTPUT
		-reset false
		-molalities Fe+2 Hfo_sOZn+
	</PRE></CODE>
	</TD>
	<TD width=35%>
	<CODE><PRE>
	  m_Fe+2(mol/kgw)  m_Hfo_sOZn+(mol/kgw)
	</PRE></CODE>
	</TD>
	</TR>

	<TR VALIGN="top">
	<TD width=65%>
	<CODE><PRE>
	  SELECTED_OUTPUT
		-reset false
		-activities H+ Ca+2
	</PRE></CODE>
	</TD>
	<TD width=35%>
	<CODE><PRE>
	  la_H+  la_Ca+2
	</PRE></CODE>
	</TD>
	</TR>

	<TR VALIGN="top">
	<TD width=65%>
	<CODE><PRE>
	  SELECTED_OUTPUT
		-reset false
		-equilibrium_phases Calcite Dolomite
	</PRE></CODE>
	</TD>
	<TD width=35%>
	<CODE><PRE>
	  Calcite  d_Calcite  Dolomite  d_Dolomite
	</PRE></CODE>
	</TD>
	</TR>

	<TR VALIGN="top">
	<TD width=65%>
	<CODE><PRE>
	  SELECTED_OUTPUT
		-reset false
		-saturation_indices CO2(g) Siderite
	</PRE></CODE>
	</TD>
	<TD width=35%>
	<CODE><PRE>
	  si_CO2(g)  si_Siderite
	</PRE></CODE>
	</TD>
	</TR>

	<TR VALIGN="top">
	<TD width=65%>
	<CODE><PRE>
	  SELECTED_OUTPUT
		-reset false
		-gases CO2(g) N2(g)
	</PRE></CODE>
	</TD>
	<TD width=35%>
	<CODE><PRE>
	  pressure "total mol" volume g_CO2(g) g_N2(g)
	</PRE></CODE>
	</TD>
	</TR>

	<TR VALIGN="top">
	<TD width=65%>
	<CODE><PRE>
	  SELECTED_OUTPUT
		-reset false
		-kinetic_reactants CH2O Pyrite
	</PRE></CODE>
	</TD>
	<TD width=35%>
	<CODE><PRE>
	  k_CH2O dk_CH2O k_Pyrite dk_Pyrite
	</PRE></CODE>
	</TD>
	</TR>

	<TR VALIGN="top">
	<TD width=65%>
	<CODE><PRE>
	  SELECTED_OUTPUT
		-reset false
		-solid_solutions CaSO4 SrSO4
	</PRE></CODE>
	</TD>
	<TD width=35%>
	<CODE><PRE>
	  s_CaSO4 s_SrSO4
	</PRE></CODE>
	</TD>
	</TR>

	</table>
	 *  @endhtmlonly
	 */
	VRESULT                  GetSelectedOutputValue(int row, int col, VAR* pVAR);

	/**
	 *  Retrieves the warning messages from the last call to \ref RunAccumulated, \ref RunFile, \ref RunString, \ref LoadDatabase, or \ref LoadDatabaseString.
     *  @return                 A null terminated string containing warning messages.
	 *  @see                    GetWarningStringLine, GetWarningStringLineCount, OutputWarningString
	 */
	const char*              GetWarningString(void);

	/**
     *  Retrieves the given warning line.
	 *  @param n                The zero-based index of the line to retrieve.
     *  @return                 A null terminated string containing the given warning line message.
	 *  @see                    GetWarningStringLineCount, OutputWarningString
	 */
	const char*              GetWarningStringLine(int n);

	/**
	 *  Retrieves the number of lines in the current warning string buffer.
     *  @return                 The number of lines.
	 *  @see                    GetWarningStringLine, GetWarningString, OutputWarningString
	 */
	int                      GetWarningStringLineCount(void)const;

	/**
	 *  Retrieves the current list of components.
	 *  @return                 The current list of components.
	 *  @see                    GetComponent, GetComponentCount
	 */
	std::list< std::string > ListComponents(void);

	/**
	 *  Load the specified database file into phreeqc.
	 *  @param filename         The name of the phreeqc database to load.
	 *                          The full path (or relative path with respect to the working directory) will be required if the file is not
	 *                          in the current working directory.
	 *  @return                 The number of errors encountered.
	 *  @see                    LoadDatabaseString
	 *  @remarks
     *      All previous definitions are cleared.
	 */
	int                      LoadDatabase(const char* filename);

	/**
	 *  Load the specified string as a database into phreeqc.
	 *  @param input            String containing data to be used as the phreeqc database.
	 *  @return                 The number of errors encountered.
	 *  @see                    LoadDatabaseString
	 *  @remarks
     *      All previous definitions are cleared.
	 */
	int                      LoadDatabaseString(const char* input);

	/**
     *  Output the accumulated input buffer to stdout.  The input buffer can be run with a call to \ref RunAccumulated.
	 *  @see                    AccumulateLine, ClearAccumulatedLines, RunAccumulated
	 */
	void                     OutputAccumulatedLines(void);

	/**
	 *  Output the error messages normally stored in the <B><I>phreeqc.err</I></B> file to stdout.
	 *  @see                    GetErrorStringLine, GetErrorStringLineCount, GetErrorFileOn, SetErrorFileOn
	 */
	void                     OutputErrorString(void);

	/**
	 *  Output the warning messages to stdout.
	 *  @see                    GetWarningStringLine, GetWarningStringLineCount, GetWarningString
	 */
	void                     OutputWarningString(void);

	/**
	 *  Runs the input buffer as defined by calls to \ref AccumulateLine.
	 *  @return                 The number of errors encountered.
	 *  @see                    AccumulateLine, ClearAccumulatedLines, OutputAccumulatedLines, RunFile, RunString
	 *  @remarks
	 *      The accumulated input is cleared at the next call to \ref AccumulateLine.
	 *  @pre
	 *      \ref LoadDatabase/\ref LoadDatabaseString must have been called and returned 0 (zero) errors.
	 */
	int                      RunAccumulated(void);

	/**
	 *  Runs the specified phreeqc input file.
	 *  @param filename         The name of the phreeqc input file to run.
	 *  @return                 The number of errors encountered during the run.
	 *  @see                    RunAccumulated, RunString
	 *  @pre
	 *      \ref LoadDatabase/\ref LoadDatabaseString must have been called and returned 0 (zero) errors.
	 */
	int                      RunFile(const char* filename);

	/**
	 *  Runs the specified string as input to phreeqc.
	 *  @param input            String containing phreeqc input.
	 *  @return                 The number of errors encountered during the run.
	 *  @see                    RunAccumulated, RunFile
	 *  @pre
	 *      \ref LoadDatabase/\ref LoadDatabaseString must have been called and returned 0 (zero) errors.
	 */
	int                      RunString(const char* input);

	/**
	 *  Sets the dump file switch on or off.  This switch controls whether or not phreeqc writes to the <B>DUMP</B> (<B><I>dump.out</I></B> if unspecified) file.
	 *  The initial setting is false.
	 *  @param bValue           If true, turns on output to the <B>DUMP</B> file;
	 *                          if false, turns off output to the <B>DUMP</B> file.
	 *  @see                    GetDumpFileOn, GetDumpString, GetDumpStringOn, GetDumpStringLine, GetDumpStringLineCount, SetDumpStringOn
	 */
	void                     SetDumpFileOn(bool bValue);

	/**
	 *  Sets the dump string switch on or off.  This switch controls whether or not the data normally sent
	 *  to the dump file are stored in a buffer for retrieval.  The initial setting is false.
	 *  @param bValue           If true, captures the output defined by the <B>DUMP</B> keyword into a string buffer;
	 *                          if false, output defined by the <B>DUMP</B> keyword is not captured to a string buffer.
	 *  @see                    GetDumpFileOn, GetDumpString, GetDumpStringOn, GetDumpStringLine, GetDumpStringLineCount, SetDumpFileOn
	 */
	void                     SetDumpStringOn(bool bValue);

	/**
	 *  Sets the error file switch on or off.  This switch controls whether or not
	 *  error messages are written to the <B><I>phreeqc.err</I></B> file.  The initial setting is false.
	 *  @param bValue           If true, writes errors to the error file; if false, no errors are written to the error file.
	 *  @see                    GetErrorStringLine, GetErrorStringLineCount, GetErrorFileOn, OutputErrorString
	 */
	void                     SetErrorFileOn(bool bValue);

	/**
	 *  Sets the log file switch on or off.  This switch controls whether or not phreeqc
	 *  writes log messages to the <B><I>phreeqc.log</I></B> file.  The initial setting is false.
	 *  @param bValue           If true, turns on output to the log file; if false, no log messages are written to the log file.
     *  @remarks
     *      Logging must be enabled through the use of the KNOBS -logfile option in order to receive any log messages.
	 *  @see                    GetLogFileOn
	 */
	void                     SetLogFileOn(bool bValue);

	/**
	 *  Sets the output file switch on or off.  This switch controls whether or not phreeqc
	 *  writes to the <B><I>phreeqc.out</I></B> file.  This is the output that is normally generated
	 *  when phreeqc is run.  The initial setting is false.
	 *  @param bValue           If true, writes output to the output file; if false, no output is written to the output file.
	 *  @see                    GetOutputFileOn
	 */
	void                     SetOutputFileOn(bool bValue);

	/**
	 *  Sets the selected-output file switch on or off.  This switch controls whether or not phreeqc writes output to
	 *  the <B>SELECTED_OUTPUT</B> (<B><I>selected.out</I></B> if unspecified) file. The initial setting is false.
	 *  @param bValue           If true, writes output to the selected-output file; if false, no output is written to the selected-output file.
	 *  @see                    GetSelectedOutputColumnCount, GetSelectedOutputFileOn, GetSelectedOutputRowCount, GetSelectedOutputValue
	 */
	void                     SetSelectedOutputFileOn(bool bValue);

public:
	// overrides
	virtual void error_msg(const char * str, bool stop=false);
	virtual void output_msg(const char * str);
	virtual void screen_msg(const char *str);
	virtual void punch_msg(const char *str);

	virtual void fpunchf(const char *name, const char *format, double d);
	virtual void fpunchf(const char *name, const char *format, char * d);
	virtual void fpunchf(const char *name, const char *format, int d);
	virtual void fpunchf_end_row(const char *format);

	virtual bool punch_open(const char *file_name);

protected:
	static int handler(const int action, const int type, const char *err_str, const int stop, void *cookie, const char *format, va_list args);
	int output_handler(const int type, const char *err_str, const int stop, void *cookie, const char *format, va_list args);
	int open_handler(const int type, const char *file_name);

	static int module_handler(const int action, const int type, const char *err_str, const int stop, void *cookie, const char *format, va_list args);
	int module_isopen_handler(const int type);
	int module_open_handler(const int type, const char *file_name);

	int output_isopen(const int type);

	int EndRow(void);
	void AddSelectedOutput(const char* name, const char* format, va_list argptr);
	void UnLoadDatabase(void);

	void check_database(const char* sz_routine);
	int close_input_files(void);
	int close_output_files(void);
	void open_output_files(const char* sz_routine);

	void do_run(const char* sz_routine, std::istream* pis, PFN_PRERUN_CALLBACK pfn_pre, PFN_POSTRUN_CALLBACK pfn_post, void *cookie);

	void update_errors(void);

protected:
	bool                       DatabaseLoaded;
	bool                       ClearAccumulated;
	bool                       UpdateComponents;
	bool                       SelectedOutputOn;
	bool                       OutputOn;
	bool                       LogOn;
	bool                       ErrorOn;
	bool                       DumpOn;
	bool                       DumpStringOn;

#if defined(_MSC_VER)
/* disable warning C4251: 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2' */
#pragma warning(disable:4251)
#endif

	IErrorReporter            *ErrorReporter;
	std::string                ErrorString;
	std::vector< std::string > ErrorLines;

	IErrorReporter            *WarningReporter;
	std::string                WarningString;
	std::vector< std::string > WarningLines;

	CSelectedOutput           *SelectedOutput;
	std::string                PunchFileName;
	std::string                StringInput;

	std::string                DumpString;
	std::vector< std::string > DumpLines;

	std::list< std::string >   Components;

#if defined(_MSC_VER)
/* reset warning C4251 */
#pragma warning(default:4251)
#endif

protected:
	Phreeqc* PhreeqcPtr;

#if defined(CPPUNIT)
	friend class TestIPhreeqc;
	friend class TestSelectedOutput;
#endif

private:
	/**
	 *  Copy constructor not supported
	 */
	IPhreeqc(const IPhreeqc&);

	/**
	 *  operator= not supported
	 */
	IPhreeqc& operator=(const IPhreeqc&);
};

#endif // INC_IPHREEQC_HPP
