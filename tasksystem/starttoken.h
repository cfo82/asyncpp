
#ifndef STARTTOKEN_H
#define STARTTOKEN_H

#include "tokenheader.h"

namespace tasks {

struct StartToken : public TokenHeader
{
	StartToken()
	:	TokenHeader(TokenHeader::Start, sizeof(StartToken))
	{
	}
};

} // namespace tasks

#endif // STARTTOKEN_H

