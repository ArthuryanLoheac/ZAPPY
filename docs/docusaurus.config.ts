import {themes as prismThemes} from 'prism-react-renderer';
import type {Config} from '@docusaurus/types';
import type * as Preset from '@docusaurus/preset-classic';

// This runs in Node.js - Don't use client-side code here (browser APIs, JSX...)

const config: Config = {
  title: 'Zappy docs',
  tagline: 'Raise, Trantorians !',
  favicon: 'img/zappy.ico',

  // Future flags, see https://docusaurus.io/docs/api/docusaurus-config#future
  future: {
    v4: true, // Improve compatibility with the upcoming Docusaurus v4
  },

  // Set the production url of your site here
  url: 'https://ArthuryanLoheac.github.io',
  // Set the /<baseUrl>/ pathname under which your site is served
  // For GitHub pages deployment, it is often '/<projectName>/'
  baseUrl: '/ZAPPY/',

  // GitHub pages deployment config.
  // If you aren't using GitHub pages, you don't need these.
  organizationName: 'ArthuryanLoheac', // Usually your GitHub org/user name.
  projectName: 'ZAPPY', // Usually your repo name.
  deploymentBranch: 'gh-pages',

  onBrokenLinks: 'warn',
  onBrokenMarkdownLinks: 'warn',

  // Even if you don't use internationalization, you can use this field to set
  // useful metadata like html lang. For example, if your site is Chinese, you
  // may want to replace "en" with "zh-Hans".
  i18n: {
    defaultLocale: 'en',
    locales: ['en'],
  },

  presets: [
    [
      'classic',
      {
        docs: {
          sidebarPath: './sidebars.ts',
          // Please change this to your repo.
          // Remove this to remove the "edit this page" links.
          editUrl:
            'https://github.com/ArthuryanLoheac/ZAPPY',
        },
        blog: false,
        theme: {
          customCss: './src/css/custom.css',
        },
      } satisfies Preset.Options,
    ],
  ],

  themeConfig: {
    // Replace with your project's social card
    image: 'img/docusaurus-social-card.jpg',
    navbar: {
      title: 'Zappy docs',
      logo: {
        alt: 'Zappy Logo',
        src: 'img/zappy.svg',
      },
      items: [
        {
          type: 'docSidebar',
          sidebarId: 'zappySidebar',
          position: 'right',
          label: "Documentation"
        },
        {
          href: 'https://github.com/ArthuryanLoheac/ZAPPY',
          position: 'left',
          label: 'Contribute'
        },
        {
          href: '/ZAPPY/doxygen/index.html',
          position: 'right',
          label: 'API Reference',
          target: '_blank',
        },
      ],
    },
    footer: {
      style: 'dark',
      links: [
        {
          title: 'Docs',
          items: [
            {
              label: 'API Reference',
              href: '/ZAPPY/doxygen/index.html',
              target: '_blank',
            },
          ],
        }
      ],
      copyright: `Copyright © ${new Date().getFullYear()} Zappytto team.`,
    },
    prism: {
      theme: prismThemes.github,
      darkTheme: prismThemes.dracula,
    },
  } satisfies Preset.ThemeConfig,
};

export default config;
