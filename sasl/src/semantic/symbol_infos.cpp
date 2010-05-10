#include <sasl/include/semantic/symbol_infos.h>
#include <sasl/enums/literal_constant_types.h>
#include <sasl/include/syntax_tree/declaration.h>

BEGIN_NS_SASL_SEMANTIC();

// some free functions.

void integer_literal_suffix( const std::string& str, bool& is_unsigned, bool& is_long ){
	is_unsigned = false;
	is_long = false;

	std::string::const_reverse_iterator ch_it = str.rbegin();
	char ch[2] = {'\0', '\0'};
	ch[0] = *ch_it;
	++ch_it;
	ch[1] = (ch_it == str.rend() ? '\0' : *ch_it);

	for ( int i = 0; i < 2; ++i ){
		switch (ch[i]){
			case 'u':
			case 'U':
				is_unsigned = true;
				break;
			case 'l':
			case 'L':
				is_long = true;
				break;
			default:
				// do nothing
				break;
		}
	}
}
void real_literal_suffix( const std::string& str, bool is_single){
	is_single = false;

	std::string::const_reverse_iterator ch_it = str.rbegin();
	if ( *ch_it == 'F' || *ch_it == 'f' ){
		is_single = true;
	}
}
////////////////////////////////

const_value_symbol_info::const_value_symbol_info()
	: const_value_symbol_info::base_type( "value symbol info" ), valtype( buildin_type_code::none ){}

void const_value_symbol_info::constant_value_literal(
	const std::string& litstr,
	literal_constant_types lctype)
{
	if (lctype == literal_constant_types::integer ){
		bool is_unsigned(false);
		bool is_long(false);
		integer_literal_suffix( litstr, is_unsigned, is_long );
		if ( is_unsigned ){
			val = boost::lexical_cast<unsigned long>(litstr);
			this->valtype = ( is_long ? buildin_type_code::_uint64 : buildin_type_code::_uint32 );
		} else {
			val = boost::lexical_cast<long>(litstr);
			this->valtype = ( is_long ? buildin_type_code::_sint64 : buildin_type_code::_sint32 );
		}
	} else if( lctype == literal_constant_types::real ){
		val = boost::lexical_cast<double>(litstr);
		bool is_single(false);
		real_literal_suffix( litstr, is_single );
		this->valtype = (is_single ? buildin_type_code::_float : buildin_type_code::_double);
	} else if( lctype == literal_constant_types::boolean ){
		val = (litstr == "true");
		this->valtype = buildin_type_code::_boolean;
	} else if( lctype == literal_constant_types::character ){
		val = litstr[0];
		this->valtype = buildin_type_code::_sint8;
	} else if( lctype == literal_constant_types::string ){
		val = litstr;
		this->valtype = buildin_type_code::none;
	}
}

buildin_type_code const_value_symbol_info::value_type() const{
	return valtype;
}
void const_value_symbol_info::value_type( buildin_type_code vtype ){
	valtype = vtype;
}

type_symbol_info::type_symbol_info(): base_type( "ref symbol info" ), ttype(type_types::none) { }

boost::shared_ptr<node> type_symbol_info::full_type() const{
	return type_node.lock();
}
void type_symbol_info::full_type( boost::shared_ptr<node> ftnode ){
	type_node = ftnode;
}
type_types type_symbol_info::type_type() const{
	return ttype;
}

void type_symbol_info::type_type( type_types ttype ){
	this->ttype = ttype;
}

variable_symbol_info::variable_symbol_info()
	: isloc(false), base_type( "variable symbol info" )
{
}
bool variable_symbol_info::is_local() const{
	return isloc;
}

void variable_symbol_info::is_local( bool isloc ){
	this->isloc = isloc;
}

END_NS_SASL_SEMANTIC();