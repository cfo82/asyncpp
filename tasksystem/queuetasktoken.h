
#ifndef QUEUETASKTOKEN_H
#define QUEUETASKTOKEN_H

namespace tasks {

struct QueueTaskToken : public TokenHeader
{
	QueueTaskToken(int index_in)
	:	TokenHeader(TokenHeader::QueueTask, sizeof(QueueTaskToken)),
		index(index_in)
	{
	}

	int index;
};

template <int TaskIndex>
struct QueueTaskTokenTemplate : public QueueTaskToken
{
	QueueTaskTokenTemplate()
	:	QueueTaskToken(TaskIndex)
	{
	}
};

} // namespace tasks

#endif // QUEUETASKTOKEN_H
