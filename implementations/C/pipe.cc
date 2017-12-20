// Algorithm to C++ generator
#ifndef __PIPE_HEADER
#define __PIPE_HEADER
class PIPE {
	public:

		// Generate interface types
		typedef float RealNumber;

		// Generate function
		bool operator () (
			const RealNumber& x,
			RealNumber& y,
			void *ctx)
		{
			// Implement algorithm here
            y = x;
			return true;
		}
};
#endif
