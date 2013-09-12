//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#include	"precompile.h"
#include	"priority_queue_of.h"
#include	"test.h"

//--------------------------------------------------------------------------
//	tests
//--------------------------------------------------------------------------
#ifdef	_RUN_TESTS
DEFINE_TEST (PriorityQueueOf)
{
	DO_AFTER (ResizableVectorOf);
}

//--------------------------------------------------------------------------
ON_TEST (PriorityQueueOf)
{
	DO_TEST_REPORT;
	PriorityQueueOf<uInt>	queue;
	uInt					array[10] = {4, 7, 2, 9, 3, 5, 0, 6, 1, 8};
	for (uInt i = 0; i < 5; i++)
		queue.Insert (array[i]);
	TEST_CONDITION (queue.GetQueueSize () == 5);
	for (i = 5; i < 10; i++)
		queue.InsertLazy (array[i]);
	TEST_CONDITION (queue.GetQueueSize () == 10);
	for (i = 0; i < 10; i++)
	{
		TEST_CONDITION (queue.GetMinimum () == i);
		TEST_CONDITION (queue.ExtractMinimum () == i);
	}
}
#endif

//--------------------------------------------------------------------------
