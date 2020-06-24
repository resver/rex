module.exports = {
  title: "Rex",
  tagline: "Ultra fast web framework for ReasonML",
  url: "https://resver.github.com/rex",
  baseUrl: "/",
  favicon: "img/favicon.ico",
  organizationName: "resver",
  projectName: "rex",
  themeConfig: {
    prism: {
      additionalLanguages: ["reason"],
    },
    navbar: {
      title: "Rex",
      logo: {
        alt: "My Site Logo",
        src: "img/rex.png",
      },
      links: [
        {
          to: "docs/",
          activeBasePath: "docs",
          label: "Guides",
          position: "right",
        },
        { to: "api/", label: "API Reference", position: "right" },
        {
          href: "https://github.com/damaera/rex",
          label: "GitHub",
          position: "right",
        },
      ],
    },
    footer: {
      style: "dark",
      links: [
        {
          title: "Docs",
          items: [
            {
              label: "Style Guide",
              to: "docs/",
            },
            {
              label: "Second Doc",
              to: "docs/doc2/",
            },
          ],
        },
        {
          title: "Community",
          items: [
            {
              label: "Stack Overflow",
              href: "https://stackoverflow.com/questions/tagged/docusaurus",
            },
            {
              label: "Discord",
              href: "https://discordapp.com/invite/docusaurus",
            },
            {
              label: "Twitter",
              href: "https://twitter.com/docusaurus",
            },
          ],
        },
        {
          title: "More",
          items: [
            {
              label: "Blog",
              to: "blog",
            },
            {
              label: "GitHub",
              href: "https://github.com/facebook/docusaurus",
            },
          ],
        },
      ],
      copyright: `Copyright © ${new Date().getFullYear()} Rex.`,
    },
  },
  presets: [
    [
      "@docusaurus/preset-classic",
      {
        docs: {
          // It is recommended to set document id as docs home page (`docs/` path).
          homePageId: "guides/getting-started",
          sidebarPath: require.resolve("./sidebars.js"),
          // Please change this to your repo.
          editUrl: "https://github.com/damaera/rex/tree/master/docs/",
        },
        blog: {
          showReadingTime: true,
          // Please change this to your repo.
          editUrl:
            "https://github.com/facebook/docusaurus/edit/master/website/blog/",
        },
        theme: {
          customCss: require.resolve("./src/css/custom.css"),
        },
      },
    ],
  ],
};
