
#ifndef ENDTOKEN_H
#define ENDTOKEN_H

namespace tasks {

struct EndToken : public TokenHeader
{
	EndToken()
	:	TokenHeader(TokenHeader::End, sizeof(StartToken))
	{
	}
};

} // namespace tasks

#endif // ENDTOKEN_H
