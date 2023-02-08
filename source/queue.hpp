/// Queue data type
typedef int queue_data_t;


/// Queue
typedef struct {
    queue_data_t *data = nullptr;       ///< Array of queue_data_t elements
    int capacity       =       0;       ///< Maximum size
    int size           =       0;       ///< Actual number of elements
    int head           =       0;       ///< Queue head pointer
} Queue;


#define POISON_VALUE 0xC0FFEE
#define MAX_CAPACITY_VALUE 100000
#define OBJECT_TO_STR "%i"


/// Exit codes for queue functions
enum EXIT_CODES {
    INVALID_DATA     =  1,        ///< Invalid queue data points
    INVALID_SIZE     =  2,        ///< Size is larger than capacity or negative
    INVALID_CAPACITY =  3,        ///< Capacity is larger than type's maximum or negative
    UNEXP_POISON_VAL =  4,        ///< Unexpected poison value before size
    UNEXP_NORMAL_VAL =  5,        ///< Unexpected normal value after size
    INVALID_ARGUMENT =  6,        ///< Invalid argument given to the function
    EMPTY_QUEUE      =  7,        ///< No elements to pop
    ALLOCATE_FAIL    =  8,        ///< Memory allocate return NULL
};


/**
 * \brief Prints queue content
 * \param [in] queue Queue to print
 * \param [in] error This error code will printed
*/
#define QUEUE_DUMP(queue, error)                                                        \
do {                                                                                    \
    fprintf(stdout, "%s at %s(%d)\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);         \
    queue_dump(queue, error, stdout);                                                   \
} while(0)


/**
 * \brief Constructs the queue
 * \param [in] queue    This queue will be filled
 * \param [in] capacity New queue capacity
 * \note Free queue before contsructor to prevent memory leak
 * \return Non zero value means error
*/
int queue_constructor(Queue *queue, int capacity);


/**
 * \brief Adds object to queue
 * \param [in] queue  This queue will be pushed
 * \param [in] object This object will be added to the end of queue
 * \note Queue will try to resize to hold all the objects
 * \return Non zero value means error
*/
int queue_push(Queue *queue, queue_data_t object);


/**
 * \brief Returns last object from queue and removes it
 * \param [in] queue  This queue will be popped
 * \param [in] object Value of popped object will be written to this pointer
 * \note Queue will try to resize if hold too few objects for its size
 * \return Non zero value means error
*/
int queue_pop(Queue *queue, queue_data_t *object);


/**
 * \brief Destructs the queue
 * \param [in] queue This queue will be destructed
 * \note Queue won't be free in case of verification error so get ready for memory leak
 * \return Non zero value means error
*/
int queue_destructor(Queue *queue);


/**
 * \brief Queue verificator
 * \param [in] queue Queue to check
 * \return Non zero value means error
*/
int queue_verificator(Queue *queue);


/**
 * \brief Prints queue content
 * \param [in]  queue  This queue will printed
 * \param [in]  error  This error code will be printed
 * \param [out] stream File to dump in
*/
void queue_dump(Queue *queue, int error, FILE *stream);
