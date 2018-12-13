#ifndef DAISY_SHADER_H
#define	DAISY_SHADER_H

namespace daisy {
	
	template<typename TAttr>
	class shader_bind_point_info
	{
	public:
		unsigned int attribute_id;
		TAttr* bind_point;
	};

}

#endif // !DAISY_SHADER_H
