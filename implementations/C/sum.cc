// Algorithm to C++ generator
#ifndef __SUM_HEADER
#define __SUM_HEADER
class SUM {
	public:

		// Generate interface types
		typedef float[10] RealNumberVec;
		typedef float RealNumber;
		typedef uint8_t RealNumberVecEntries;

		// Generate function
		bool operator () (
			const RealNumberVec& values,
			const RealNumber& defaultValue,
			const RealNumberVecEntries& entries,
			const RealNumber& bias,
			const RealNumberVec& weights,
			RealNumber& merged,
			void *ctx)
		{
			// Implement algorithm here
            if (entries > 0)
            {
                merged = bias;
                for (uint8_t i = 0; i < entries; ++i)
                    merged += values * weights;
            } else {
                merged = defaultValue;
            }
			return true;
		}
};
#endif
