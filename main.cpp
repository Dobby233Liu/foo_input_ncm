#include "common.h"
#include "pch.h"

class ncm_initquit : public initquit {
public:
	void on_init() {}
	void on_quit() {}
};

static initquit_factory_t<ncm_initquit> g_myinitquit_factory;

class input_ncm : public input_stubs {
public:
	void open(service_ptr_t<file> p_filehint, const char* p_path, t_input_open_reason p_reason, abort_callback& p_abort) {
		if (p_reason == input_open_info_write) throw exception_io_unsupported_format();//our input does not support retagging.
		m_file = p_filehint;//p_filehint may be null, hence next line
		input_open_file_helper(m_file, p_path, p_reason, p_abort);//if m_file is null, opens file with appropriate privileges for our operation (read/write for writing tags, read-only otherwise).
	}

	void get_info(file_info& p_info, abort_callback& p_abort) {
		// TODO: NCMLIB
	}
	t_filestats get_file_stats(abort_callback& p_abort) { return m_file->get_stats(p_abort); }

	void decode_initialize(unsigned p_flags, abort_callback& p_abort) {
		// m_file->reopen(p_abort);
		// TODO: NCMLIB
		throw exception_io_unsupported_format();
	}
	bool decode_run(audio_chunk& p_chunk, abort_callback& p_abort) {
		// TODO: NCMLIB
		throw exception_io_unsupported_format();
	}
	void decode_seek(double p_seconds, abort_callback& p_abort) {
		// m_file->ensure_seekable();//throw exceptions if someone called decode_seek() despite of our input having reported itself as nonseekable.
		// TODO: NCMLIB
		// m_file->seek(target, p_abort);
		throw exception_io_unsupported_format();
	}
	bool decode_can_seek() { return m_file->can_seek(); }
	bool decode_get_dynamic_info(file_info& p_out, double& p_timestamp_delta) { return false; } // deals with dynamic information such as VBR bitrates
	bool decode_get_dynamic_info_track(file_info& p_out, double& p_timestamp_delta) { return false; } // deals with dynamic information such as track changes in live streams
	void decode_on_idle(abort_callback& p_abort) { m_file->on_idle(p_abort); }

	void retag(const file_info& p_info, abort_callback& p_abort) { throw exception_io_unsupported_format(); }

	static bool g_is_our_content_type(const char* p_content_type) { return false; } // match against supported mime types here
	static bool g_is_our_path(const char* p_path, const char* p_extension) { return stricmp_utf8(p_extension, "ncm") == 0; }
	static const char* g_get_name() { return "NetEase CloudMusic copyright protection audio data"; }
	static const GUID g_get_guid() {
		// GUID of the decoder. Replace with your own when reusing code.
		static const GUID guid = pfc::GUID_from_text("f1fa0981756449969987d26e5f970abd");
		return guid;
	}
public:
	service_ptr_t<file> m_file;
	pfc::array_t<t_uint8> m_buffer;
};
static input_singletrack_factory_t<input_ncm> g_input_ncm_factory;

DECLARE_FILE_TYPE("NetEase CloudMusic-format music", "*.NCM");
DECLARE_COMPONENT_VERSION("NCM Decoder", "0.1", "Decodes NetEase CloudMusic copyright protection audio data");