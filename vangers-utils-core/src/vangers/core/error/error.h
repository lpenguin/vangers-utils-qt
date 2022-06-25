#ifndef ERROR_H
#define ERROR_H
#include <QString>
#include <optional>

namespace vangers::core::error {
	struct Error {
		QString message;
	};

	class Result {
	public:
		Result()
			: _error()
			, _isOk(true){
		}

		Result(const Error& error)
			: _error(error)
			, _isOk(false){
		}

		Result(const Result& result) = default;
		Result(Result&& result) = default;

		operator bool() const {
			return _isOk;
		}


		bool isOk() const {
			return _isOk;
		}

		const Error& error() const {
			return _error;
		}

	private:
		Error _error;
		bool _isOk;
	};
}


#endif // ERROR_H
