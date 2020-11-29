#include"Log.h"

namespace Log
{
	LOG getlog(wchar_t logfile[])				// ������������ ��������� LOG
	{
		Log::LOG log;
		wcscpy_s(log.logfile, logfile);

		log.stream = new std::ofstream;

		log.stream->open(logfile);
		if (!log.stream->is_open())
			ERROR_THROW(112);

		return log;
	}
	void WriteLine(LOG log, char* c, ...)		// ������� � �������� ������������ �����
	{
		va_list args;
		std::string text;
		va_start(args, c);
		for (char* i = c; i != ""; i = va_arg(args, char*))
			text += i;
		va_end(args);

		*log.stream << text;
	}
	void WriteLine(LOG log, wchar_t c[], ...)	// ������� � �������� ������������ �����
	{
		va_list args;
		wchar_t text[PARM_MAX_SIZE];
		va_start(args, c);
		for (wchar_t* i = c; i != L""; i = va_arg(args, wchar_t*))
		{
			wcscat_s(text, c);
		}
		va_end(args);

		char str[PARM_MAX_SIZE];
		sprintf_s(str, "%wc", text);

		*log.stream << str;
	}
	void WriteLog(LOG log)						// ������� � �������� ���������
	{
		struct tm* loacletime(const time_t * ptm);

		const time_t timer = time(0);
		struct tm* u = new tm;	localtime_s(u, &timer);
		*log.stream << "\n---- ������� ------ ����: "
			<< u->tm_mday << '.' << u->tm_mon + 1 << '.' << u->tm_year + 1900 << ' '
			<< u->tm_hour << ':' << u->tm_min << ':' << u->tm_sec << '\n';
	}
	void WriteParm(LOG log, Parm::PARM parm)	// ������� � �������� ���������� � ������� ����������
	{
		*log.stream << "---- ��������� ------\n";
		char text[PARM_MAX_SIZE];

		sprintf_s(text, "%ws", parm.log);
		*log.stream << "-log: " << text << '\n';

		sprintf_s(text, "%ws", parm.out);
		*log.stream << "-out: " << text << '\n';

		sprintf_s(text, "%ws", parm.in);
		*log.stream << "-in: " << text << '\n';
	}
	void WriteIn(LOG log, In::IN in)			// ������� � �������� ���������� � ������� ������
	{
		*log.stream << "---- �������� ������ ------\n"
			<< "���������� ��������: " << in.size << '\n'
			<< "����������������: " << in.ignor << '\n'
			<< "���������� �����: " << in.lines << '\n';
	}
	void WriteError(LOG log, Error::ERROR error)	// ������� � �������� ���������� �� �������
	{
		*log.stream << "������ " << error.id << ": " << error.message << "(" << error.intext.line << ", " << error.intext.col << ")\n";
	}
	void Close(LOG log)							// ������� ��������
	{
		log.stream->close();
	}
}