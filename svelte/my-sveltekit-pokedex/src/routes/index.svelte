<!-- Loading data on the server side: SSR -->
<script context="module">
  export async function load() {
    const url = `https://pokeapi.co/api/v2/pokemon?limit=150`;
    const res = await fetch(url);
    const data = await res.json();
    const pokemon = data.results.map((data, index) => {
      return {
        name: data.name,
        id: index + 1,
        image: `https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/${
          index + 1
        }.png`
      };
    });

    return { props: { pokemon } };
  }
</script>

<script>
  // Loading data from the client side (navigator)
  // import {pokemon} from "../stores/pokestore";

  // the 'pokemon' comes from the <script context="module">
  export let pokemon;

  import PokemanCard from '../components/pokemanCard.svelte';

  let searchTerm = '';
  let filteredPokemon = [];

  // As a 'useEffect' in React
  $: {
    console.log(searchTerm);
    if (searchTerm) {
      filteredPokemon = pokemon.filter((pokeman) =>
        pokeman.name.toLowerCase().includes(searchTerm.toLowerCase())
      );
    } else {
      filteredPokemon = [...pokemon];
    }
  }
</script>

<svelte:head>
  <title>Svelte Kit Podekex</title>
</svelte:head>

<h1 class="text-4xl text-center my-8 uppercase">Svelte Kit Pokedex</h1>

<input
  class="w-full rounded-md text-lg p-4 border-2 border-gray-200"
  type="text"
  bind:value={searchTerm}
  placeholder="Search Pokemon"
/>

<div class="py-4 grid gap-4 md:grid-cols-2 grid-cols-1">
  {#each filteredPokemon as pokeman}
    <PokemanCard {pokeman} />
  {/each}
</div>
