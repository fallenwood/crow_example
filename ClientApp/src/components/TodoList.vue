<script setup lang="ts">
import { ref } from 'vue'
import Todo from './Todo.vue'
import Form from './Form.vue'
import {Response} from '../models/response'

const todos = ref([])

async function fetchTodos() {
  const response = await fetch("/api/todo/list");
  const data = await response.json();

  todos.value = data.data;
}

fetchTodos();
</script>

<template>
<div v-for="todo in todos">
  <Todo :todo=todo :fetchTodos=fetchTodos />
  <hr />
</div>
<Form :fetchTodos=fetchTodos />
</template>

<style scoped>
</style>
