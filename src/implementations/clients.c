#include "../includes/clients.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

bruh_node *clients = NULL;

void bruh_add_client(bruh_client *new_client) {
    bruh_node *new_node = (bruh_node *) malloc(sizeof(bruh_node));
    if(new_node == NULL) {
        printf("could not add new client: out of memory\n");
        exit(1);
    }
    // add to head of the list
    new_node->data = new_client;
    new_node->next = clients;
    clients = new_node;
}

void bruh_remove_client(bruh_client *old_client) {
    if(clients && 
        clients->data == old_client) {
        bruh_node *new_clients = clients->next;
        free(clients);
        clients = new_clients;
        return;
    }
    bruh_node *current_node = NULL;
    bruh_node *previous_node = clients;
    while(previous_node) {
        if(previous_node->next && 
            ((bruh_node *) previous_node->next)->data == old_client) {
            current_node = previous_node->next;
            break;
        }
        previous_node = previous_node->next;
    }
    if(!current_node) {
        printf("tried to remove non-existent client\n");
        exit(1);
    }
    previous_node->next = current_node->next;
    free(current_node);
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

bruh_client *bruh_get_client_by_child(Window child) {
    bruh_node *node = clients;
    while(node) {
        printf("node data: %lu", node->data->frame->window);
        if(node->data && 
            node->data->child && 
            node->data->child == child) {
            return node->data;
        }
        node = node->next;
    }
    return NULL;
}