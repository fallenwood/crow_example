<script setup lang="ts">
import {ref} from 'vue';
import TodoList from './components/TodoList.vue'

</script>

<template>
  <h1>Todo + Crow</h1>
  <TodoList />
</template>

<style scoped>
.logo {
  height: 6em;
  padding: 1.5em;
  will-change: filter;
  transition: filter 300ms;
}
.logo:hover {
  filter: drop-shadow(0 0 2em #646cffaa);
}
.logo.vue:hover {
  filter: drop-shadow(0 0 2em #42b883aa);
}
</style>
