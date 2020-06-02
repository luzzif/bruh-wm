#include "../includes/clients.h"
#include <stdlib.h>
#include <stdio.h>

bruh_node *clients = NULL;

void bruh_add_client(bruh_client *new_client) {
    bruh_node *new_node = (bruh_node *) malloc(sizeof(bruh_node));
    if(new_node == NULL) {
        printf("could not add new client: out of memory\n");
        exit(1);
    }
    new_node->data = new_client;
    new_node->next = clients;
    clients = new_node;
}

bruh_client *bruh_get_client_by_frame(Window frame) {
    bruh_node *node = clients;
    while(node) {
        if(node->data && 
            node->data->frame && 
            node->data->frame->window == frame) {
            return node->data;
        }
        node = node->next;
    }
    return NULL;
}