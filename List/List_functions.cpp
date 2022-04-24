#define $ fprintf(stderr, ">>> %d:\n", __LINE__);
#define $$(cmd)  { fprintf(stderr, ">>> %d: %s\n", __LINE__, #cmd); cmd; }

#include "List.h"

void check_nullptr(struct Plist* list)
{
    if (list == nullptr)
    {
        FILE* log = fopen("log.txt", "a");
        assert(log != nullptr);

        fprintf(log, "List [NULL_PTR_ERROR #%d] [0x000000]\n ", ERR_NULLPTR);

        fflush(log);

        FATAL_ERROR;
    }
}


void plist_free_fill(struct Plist* list, size_t start_elemenet_index, size_t count)
{
    #ifdef DEBUG_LVL_2

    check_nullptr(list);

    #endif

    for (int i = start_elemenet_index; i < count; i++)
    {
        list->data[i].prev = -1;
        list->data[i].next = i + 1;
        list->data[i].value = Otrava;
    }

    list->data[count].value = Otrava;
    list->data[count].prev  = -1;
    list->data[count].next  = 0;

}


void plist_constructor(struct Plist* list, size_t user_capacity)
{
    #ifdef DEBUG_LVL_2

    check_nullptr(list);

    #endif

    if (user_capacity < 1)
    {
        printf("ERROR:%d wrong capacity\n\n", OUT_OF_RANGE_NUMBER);
        list->err = OUT_OF_RANGE_NUMBER;
        return;
    }

    list->capacity = user_capacity;
    list->free_el_index = 1;
    list->sort = SORTED;
    list->err = OK;
    list->size = 0;

    list->data = (Plist_t*)calloc(1 + user_capacity, sizeof(Plist_t));

    if (list->data == nullptr)
    {
        printf("Memory error, too a lot capacity\n\n");
        list->err = ERR_HUGE_CAPACITY;
        return;
    }
    
    assert(list->data != nullptr);

    list->head = 0;
    list->tail = 0;
    list->data[0].next = 0;
    list->data[0].prev = 0;
    list->data[0].value = Otrava;    

    plist_free_fill(list, 1, list->capacity);

}


void plist_destructor(struct Plist* list)
{
    #ifdef DEBUG_LVL_2

    check_nullptr(list);

    #endif

    free(list->data);

    list->capacity      = Otrava;
    list->err           = Otrava;
    list->free_el_index = Otrava;
    list->head          = Otrava;
    list->tail          = Otrava;
    list->size          = Otrava;

    list->data = nullptr;

}


size_t plist_insert_start(struct Plist* list, element_t value)
{
    #ifdef DEBUG_LVL_2

    check_nullptr(list);

    #endif

    #ifdef DEBUG_LVL_1

    VERIFICATION(list);  

    #endif
    
    size_t pos = list->free_el_index;

    list->free_el_index = list->data[pos].next;

    list->data[pos].next  = 0;
    list->data[pos].prev  = 0;
    list->data[pos].value = value;

    list->size++;

    list->head = pos;
    list->tail = pos;

    #ifdef DEBUG_LVL_1

    VERIFICATION(list);  

    #endif

    return pos;
}


size_t plist_insert_first(struct Plist* list, element_t value)
{
    #ifdef DEBUG_LVL_2

    check_nullptr(list);

    #endif

    #ifdef DEBUG_LVL_1

    VERIFICATION(list);  

    #endif

    if (list->size == list->capacity)
    {
        plist_resize(list);
    }


    if (list->size == 0)
    {
        return(plist_insert_start(list, value));
    }

    size_t pos = list->free_el_index;

    list->free_el_index = list->data[pos].next;

    list->data[pos].next  = list->head;
    list->data[pos].prev  = list->data[list->head].prev;
    list->data[pos].value = value;

    list->data[list->head].prev = pos;

    list->size++;
    list->head = pos;
    list->sort = NOT_SORTED;

    #ifdef DEBUG_LVL_1

    VERIFICATION(list);  

    #endif

    return pos;
}


size_t plist_insert_last(struct Plist* list, element_t value)
{
    #ifdef DEBUG_LVL_2

    check_nullptr(list);

    #endif

    #ifdef DEBUG_LVL_1

    VERIFICATION(list);  

    #endif

    if (list->size == list->capacity)
    {
        plist_resize(list);
    }

    if (list->size == 0)
    {
        return(plist_insert_start(list, value));
    }

    size_t pos = list->free_el_index;


    list->free_el_index = list->data[pos].next;

    list->data[pos].next        = 0;
    list->data[pos].prev        = list->tail;
    list->data[pos].value       = value;

    list->data[list->tail].next = pos;

    list->size++;

    list->tail = pos;

    #ifdef DEBUG_LVL_1

    VERIFICATION(list);  

    #endif

    return pos;
}


size_t plist_insert_before(struct Plist* list, element_t value, size_t number)
{
    #ifdef DEBUG_LVL_2

    check_nullptr(list);

    #endif

    #ifdef DEBUG_LVL_1

    VERIFICATION(list);  

    #endif

    if (list->size == list->capacity)
    {
        plist_resize(list);
    }


    if (list->size == 0)
    {
        return(plist_insert_start(list, value));
    }

    if (number == list->head)
    {
        return(plist_insert_first(list, value));
    }

    if (number > list->capacity)
    {
        printf("Error: Wrong number, more than capacity\n\n");
        return OUT_OF_RANGE_NUMBER;    
    }

    size_t pos = list->free_el_index;

    list->free_el_index = list->data[pos].next;

    list->data[pos].next  = number;
    list->data[pos].prev  = list->data[number].prev;
    list->data[pos].value = value;

    list->data[list->data[number].prev].next = pos;
    list->data[number].prev = pos;
    

    list->size++;
    list->sort = NOT_SORTED;

    #ifdef DEBUG_LVL_1

    VERIFICATION(list);  

    #endif

    return pos;
}


size_t plist_insert_after(struct Plist* list, element_t value, size_t number)
{
    #ifdef DEBUG_LVL_2

    check_nullptr(list);

    #endif

    #ifdef DEBUG_LVL_1

    VERIFICATION(list);  

    #endif

    if (list->size == list->capacity)
    {
        plist_resize(list);
    }

    if (list->size == 0)
    {
        return(plist_insert_start(list, value));
    }

    if (number == list->tail)
    {
        return(plist_insert_last(list, value));
    }

    if (number > list->capacity)
    {
        printf("Error: Wrong number, more than capacity\n\n");
        return OUT_OF_RANGE_NUMBER;    
    }

    size_t pos = list->free_el_index;

    list->free_el_index = list->data[pos].next;

    list->data[pos].next  = list->data[number].next;
    list->data[pos].prev  = number;
    list->data[pos].value = value;

    list->data[list->data[number].next].prev = pos;
    list->data[number].next                  = pos;

    list->size++;
    list->sort = NOT_SORTED;

    #ifdef DEBUG_LVL_1

    VERIFICATION(list);  

    #endif

    return pos;
}


void plist_delete_el(struct Plist* list, size_t number)
{
    #ifdef DEBUG_LVL_2

    check_nullptr(list);

    #endif

    #ifdef DEBUG_LVL_1

    VERIFICATION(list);  

    #endif

    if (number != list->tail)
    {
        list->sort = NOT_SORTED;
    }

    if (list->size == 1)
    {
        list->head = 0;
        list->tail = 0;
    }

    if (list->tail == number)
    {
        list->data[list->data[number].prev].next = 0;
        list->tail = list->data[number].prev;
    }
    else
    {
        if (list->head == number)
        {
            list->data[list->data[number].next].prev = 0;
            list->head = list->data[number].next;
        }
        else
        {
            list->data[list->data[number].next].prev = list->data[number].prev;
            list->data[list->data[number].prev].next = list->data[number].next;
        }
    }

    list->data[number].value = Otrava;
    list->data[number].next = list->free_el_index;
    list->data[number].prev = -1;

    list->free_el_index = number;

    list->size--;

    #ifdef DEBUG_LVL_1

    VERIFICATION(list);  

    #endif
}


struct Plist_t* plist_resize(struct Plist* list)
{
    #ifdef DEBUG_LVL_2

    check_nullptr(list);

    #endif

    #ifdef DEBUG_LVL_1

    VERIFICATION(list);  

    #endif

    Plist_t* tmp_pointer = (Plist_t*)realloc(list->data, sizeof(Plist_t) * (list->capacity + 1 + Add_capacity) );

    if (tmp_pointer == nullptr)
    {
        printf("Memory if full. New element isn't added.\n\n");

        list->err = ERR_NULLPTR;

        return nullptr;
    }
    else
    {
        list->data = tmp_pointer;

        plist_free_fill(list, list->capacity + 1, Add_capacity + list->capacity);

        list->free_el_index = list->capacity + 1;
        list->capacity += Add_capacity;

    }

    #ifdef DEBUG_LVL_1

    VERIFICATION(list);  

    #endif

    return list->data;
}

size_t verificator(struct Plist* list)
{
    #ifdef DEBUG_LVL_2

    check_nullptr(list);

    #endif

    if (list->capacity < 0)
    {
        list->err = OUT_OF_RANGE_NUMBER;
        return OUT_OF_RANGE_NUMBER;
    }


    if ( (list->data[0].value != Otrava) || (list->data[0].prev != 0) || (list->data[0].next != 0) )
    {
        list->err = ERR_WRONG_ADDRESSING;
        return ERR_WRONG_ADDRESSING;
    }

    for (int i = 1; i <= list->capacity; i++)
    {
        if ( (list->data[i].value == Otrava) && (list->data[i].prev != -1))
        {
            list->err = ERR_WRONG_FREE_ARG;
            return ERR_WRONG_FREE_ARG;
        }
    }

    if ( (list->data[list->head].prev != 0) && (list->head != 0) )
    {
        list->err = ERR_WRONG_HEAD;
        return ERR_WRONG_HEAD;
    }

    if ( (list->data[list->tail].next != 0) && (list->tail != 0) )
    {
        list->err = ERR_WRONG_TAIL;
        return ERR_WRONG_TAIL;
    }

    if (list->capacity < list->size)
    {
        list->err = ERR_SIZE_MO_CAPACITY;
        return ERR_SIZE_MO_CAPACITY;
    }

    if (list->data == nullptr)
    {
        list->err = ERR_NULLPTR;
        return ERR_NULLPTR;
    }

    if (list->size != 0)
    {
        int next_index = list->head; 

        int count = 1;

        while (next_index != list->tail && count <= list->capacity)
        {
            next_index = list->data[next_index].next;
            count++;
        }

        if (next_index != list->tail)
        {
            list->err = ERR_WRONG_ADDRESSING_TAIL;
            return ERR_WRONG_ADDRESSING_TAIL;
        }
            

        next_index = list->tail;

        count = list->size;

        while (next_index != list->head && count > 0)
        {
            next_index = list->data[next_index].prev;
            count--;
        }


        if (next_index != list->head)
        {
            list->err = ERR_WRONG_ADDRESSING_HEAD;
            return ERR_WRONG_ADDRESSING_HEAD;
        }

    }

    return OK;
}


void case_err(struct Plist* list)
{
    #ifdef DEBUG_LVL_2

    check_nullptr(list);

    #endif

    switch (list->err)
    {
    case OUT_OF_RANGE_NUMBER:
        plist_print_err(list, OUT_OF_RANGE_NUMBER);
        break;

    case ERR_NULLPTR:
        plist_print_err(list, ERR_NULLPTR);
        break;
    
    case ERR_HUGE_CAPACITY:
        plist_print_err(list, ERR_HUGE_CAPACITY);
        break;

    case ERR_WRONG_FREE_ARG:
        plist_print_err(list, ERR_WRONG_FREE_ARG);
        break;

    case ERR_WRONG_HEAD:
        plist_print_err(list, ERR_WRONG_HEAD);
        break;

    case ERR_WRONG_TAIL:
        plist_print_err(list, ERR_WRONG_TAIL);
        break;

    case ERR_SIZE_MO_CAPACITY:
        plist_print_err(list, ERR_SIZE_MO_CAPACITY);
        break;

    case ERR_WRONG_ADDRESSING:
        plist_print_err(list, ERR_WRONG_ADDRESSING);
        break;

    case ERR_WRONG_ADDRESSING_HEAD:
        plist_print_err(list, ERR_WRONG_ADDRESSING_HEAD);
        break;

    case ERR_WRONG_ADDRESSING_TAIL:
        plist_print_err(list, ERR_WRONG_ADDRESSING_TAIL);
        break;

    default:
        break;
    }
}


void plist_print_err(struct Plist* list, size_t error)
{
    #ifdef DEBUG_LVL_2

    check_nullptr(list);

    #endif

    printf("List (ERROR #%lx: memory cell: [0x%x] \n"
           "{\n"
            "\tsize = %lu\n" 
            "\tcapacity = %lu\n"
            "\thead = %lu\n" 
            "\ttail = %lu\n"  
            "\tdata[0x%x]\n"
            "\tfirst_free_element = %lu\n"
            "}\n\n\n",  
            error, list->data, list->size,  
            list->capacity, list->head, list->tail, 
            list->data, list->free_el_index       );
}

void plist_sorted(struct Plist* list)
{   
    #ifdef DEBUG_LVL_2

    check_nullptr(list);

    #endif

    #ifdef DEBUG_LVL_1

    VERIFICATION(list);  

    #endif

    Plist_t* new_pointer = (Plist_t*) calloc (list->capacity + 1, sizeof(Plist_t));

    new_pointer[0].next  = 0;
    new_pointer[0].prev  = 0;
    new_pointer[0].value = Otrava;

    for (size_t index = 1, current_element = list->head; index <= list->size; ++index, current_element = (list->data)[current_element].next)
    {
        new_pointer[index].value = (list->data)[current_element].value;
    }


    for (size_t index = 1; index <= list->size; ++index)
    {
        new_pointer[index].next = (index + 1) % (list->size + 1);
        new_pointer[index].prev = (index - 1);
    }

    list->head = 1;
    list->tail = list->size;

    plist_free_fill(list, list->size + 1, list->capacity);

    list->free_el_index = list->size + 1;

    Plist_t* old_pointer = list->data;
    
    list->data = new_pointer;

    free(old_pointer);

    #ifdef DEBUG_LVL_1

    VERIFICATION(list);  

    #endif
}

void plist_dump(struct Plist* list, FILE* file)
{
    #ifdef DEBUG_LVL_2

    check_nullptr(list);

    #endif

    assert(file != nullptr);

    case_err(list);

    fprintf(file, "List (ERROR #%x: memory cell: [0x%x] \n"
                    "{\n"
                    "\tsize = %lu\n" 
                    "\tcapacity = %lu\n"
                    "\thead = %lu\n" 
                    "\ttail = %lu\n"  
                    "\tdata[0x%x]\n"
                    "\tfirst_free_element = %lu\n",  
                    list->err, list->data, list->size,  
                    list->capacity, list->head, list->tail, list->data, list->free_el_index);
    
    for (int i = 0; i <= list->capacity; i++)
    {
        fprintf(file, "\tdata[%d].next = %lu \t", i, list->data[i].next);
    }   
    
    fprintf(file, "\n");

    for (int i = 0; i <= list->capacity; i++)
    {
        fprintf(file, "\tdata[%d].prev = %lu \t", i, list->data[i].prev);
    }

    fprintf(file, "\n");

    for (int i = 0; i <= list->capacity; i++)
    {
        fprintf(file, "\tdata[%d].value = %d \t", i, list->data[i].value);
    } 
    fprintf(file, "\n\n\n");
    
    fflush(file);
}

void plist_graph(Plist* list) 
{

    assert (list != nullptr);

    #ifdef DEBUG_LVL_1

    VERIFICATION(list);  

    #endif

    FILE *dump_file = fopen("graph.txt", "w");

    printf("%p\n", dump_file);

    fputs("digraph structs {\n", dump_file);

    fputs("    node [color=red, shape=Msquare, style=\"rounded, filled\"];\n", dump_file);
    
    fputs("    rankdir=LR;\n", dump_file);
  
    fprintf(dump_file, "    front [fillcolor=\"%s\", "
                       "    label=\"HEAD = %lu\"];\n",
                       "#40FD14", list->head);

    fprintf(dump_file, "    back  [fillcolor=\"%s\", "
                       "    label=\"TAIL = %lu\"];\n",
                       "#40FD14", list->tail);

    fprintf(dump_file, "    free  [fillcolor=\"%s\", "
                       "    label=\"FREE = %lu\"];\n",
                       "#40FD14", list->free_el_index);

    fprintf(dump_file, "    sorted [fillcolor=\"%s\","
                       "    label=\"SORTED = %d\"];\n",
                       "#40FD14", list->sort);

    fprintf(dump_file, "    size   [fillcolor=\"%s\","
                       "    label=\"SIZE = %lu\"];\n",
                       "#40FD14", list->size);                     

    fputs("    node [color=red, style=\"rounded, filled\"];\n", dump_file);

    fputs("\n", dump_file);
    
    fputs("    edge [style=invis, constraint=true];\n", dump_file);

    for (size_t index = 0; index <= list->capacity; ++index) 
    {
        fprintf(dump_file, "    node%lu [fillcolor=\"%s\","
                           "    label=\" %lu \\n  p = %lu \\n %d \\n n = %lu\"];\n",
                                index, "#40FD14", index,
                                (list->data[index].prev == -1) ? -1 : list->data[index].prev,
                                list->data[index].value, list->data[index].next);
        if (index > 0)
        {
            fprintf(dump_file, "    node%lu -> node%lu;\n", index - 1, index);
        }
    }   

    fputs("\n    edge [style=solid, constraint=false];\n", dump_file);

    for (size_t index = 1; index <= list->capacity; ++index) 
    {
        if (index != list->size) 
        {
        if (list->data[index].next != 0)
        {
        fprintf(dump_file, "    node%lu: <n> -> node%lu;\n", index, list->data[index].next);
        }

        if (list->data[index].prev != -1 && list->data[index].prev != 0)  
        {
        fprintf(dump_file, "    node%lu: <p> -> node%lu;\n", index, list->data[index].prev);
        }

        fputs("\n", dump_file);
        
        }
    }

    fprintf(dump_file, "    front -> node%lu; \n", list->head);
    fprintf(dump_file, "    back  -> node%lu; \n", list->tail );
    fprintf(dump_file, "    free  -> node%lu; \n", list->free_el_index );

    fputs("}\n", dump_file);

    fclose(dump_file);

    //system (".\\graphviz\\bin\\dot -Tpng graph -o dump.png");
    
    system ("dot -Tpng graph.txt -o dump.png");

    //system (".bin\\dot -Tpng demo.dot -o dump.png");
    
    //system (".bin\\eog ..\\a.png");
    //system("dump.png");

    return;
}
