#include "node.h"
#include "containsdependentnodeid.h"
#include "constructinitialnodelist.h"
#include "constructparentnodelist.h"

template <int id>
struct NodeId
{
	static const int Id = id;
};


/*int main(const int argc, const char* argv)
{
	return EXIT_SUCCESS;
}
*/