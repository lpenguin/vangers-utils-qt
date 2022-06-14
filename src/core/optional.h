#ifndef OPTIONAL_H
#define OPTIONAL_H
namespace vangers::core::optional {

	template<typename T>
	class Optional{
	public:
		Optional(const T& value)
			: _value(value)
			, _hasValue(true)
		{}

		Optional()
			: _value()
			, _hasValue(false)
		{}

		Optional(const Optional&) = default;
		Optional(Optional&&) = default;
		Optional& operator=(const Optional&) = default;
		Optional& operator=(Optional&&) = default;

		bool hasValue() const {return _hasValue;}

		const T& valueConst() const { return _value; }

		T& value() { return _value; }

		T valueOrDefault(const T& defautlValue = T{}) const {
			return _hasValue ? _value : defautlValue;
		}

		operator T() const {
			return _value;
		}

	private:
		T _value;
		bool _hasValue;
	};
}
#endif // OPTIONAL_H
